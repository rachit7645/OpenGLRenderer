#include "ShadowMap.h"

#include <limits>
#include <vector>

#include "Util.h"
#include "RenderConstants.h"
#include "FBOAttachment.h"
#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Entities::Camera;
using Engine::Settings;

// Aliases
using Vec4s = ShadowMap::Vec4s;
using Mat4s = ShadowMap::Mat4s;

// Constants
constexpr glm::vec4 MAP_BORDER = {1.0f, 1.0f, 1.0f, 1.0f};

// Shadow levels vector
const std::vector<f32> shadowLevels =
{
	FAR_PLANE / 50.0f,
	FAR_PLANE / 25.0f,
	FAR_PLANE / 10.0f,
	FAR_PLANE / 2.0f
};

ShadowMap::ShadowMap()
	: buffer(std::make_shared<FrameBuffer>()),
	  m_matrixBuffer(std::make_shared<ShadowBuffer>())
{
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Depth attachment
	Renderer::FBOAttachment depth =
	{
        GL_LINEAR,
        GL_LINEAR,
		GL_CLAMP_TO_BORDER,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_DEPTH_ATTACHMENT,
		const_cast<f32*>(&MAP_BORDER[0])
	};

	// Set buffer dimensions
	buffer->width  = settings.shadows.dimensions.x;
	buffer->height = settings.shadows.dimensions.y;
	buffer->depth  = static_cast<GLsizei>(shadowLevels.size() + 1);

	// Create frame buffer object
	buffer->CreateFrameBuffer();
	buffer->Bind();
	// Set color buffer to none
	buffer->SetDrawBuffer(GL_NONE);
	buffer->SetReadBuffer(GL_NONE);
	// Create depth texture
	buffer->AddTextureArray(buffer->depthTexture, depth);
	// Finish up
	buffer->CheckStatus();
	buffer->EnableDepth();
	buffer->Unbind();

    // Bind depth texture
    buffer->depthTexture->Bind();
    // Enable depth comparison mode
    buffer->depthTexture->SetParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    // Set depth compare mode
    buffer->depthTexture->SetParameter(GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    // Unbind depth texture
    buffer->depthTexture->Bind();

	// Load cascade distances
	m_matrixBuffer->LoadDistances(shadowLevels);
}

void ShadowMap::Update(const Camera& camera, const glm::vec3& lightPos)
{
	// Calculate cascade matrices
	auto matrices = CalculateLightSpaceMatrices(camera, glm::normalize(-lightPos));
	// Load updated cascade matrices
	m_matrixBuffer->LoadMatrices(matrices);
}

Mat4s ShadowMap::CalculateLightSpaceMatrices
(
	const Camera& camera,
	const glm::vec3& lightDir
)
{
	// Pre-allocate memory
	auto matrices = Mat4s(shadowLevels.size() + 1);

	// Calculate in between
	for (usize i = 1; i < shadowLevels.size(); ++i)
	{
		matrices[i] = CalculateLightSpaceMatrix(shadowLevels[i - 1], shadowLevels[i], camera, lightDir);
	}

	// Calculate front and back
	matrices.front() = CalculateLightSpaceMatrix(NEAR_PLANE,          shadowLevels.front(), camera, lightDir);
	matrices.back()  = CalculateLightSpaceMatrix(shadowLevels.back(), FAR_PLANE,            camera, lightDir);

	// Return
	return matrices;
}

glm::mat4 ShadowMap::CalculateLightSpaceMatrix
(
	f32 nearPlane,
	f32 farPlane,
	const Camera& camera,
	const glm::vec3& lightDir
)
{
	// Projection needs to be the same as the main projection matrix
	glm::mat4 proj = glm::perspective(glm::radians(FOV), ASPECT_RATIO, nearPlane, farPlane);
	// View matrix needs to be the same as the main one
	Vec4s corners = CalculateFrustumCorners(proj, camera.GetViewMatrix());
	// Light view and projection
	glm::mat4 lightView = CalculateViewMatrix(corners, lightDir);
	glm::mat4 lightProj = CalculateProjMatrix(corners, lightView);
	// Return
	return lightProj * lightView;
}

glm::vec3 ShadowMap::CalculateCenter(const Vec4s& corners)
{
	// Set center to 0 as default
	auto center = glm::vec3(0.0f);
	// For all corners of the view frustum
	for (const auto& corner : corners)
	{
		// Add corner to the center
		center += glm::vec3(corner);
	}
	// Divide by the corner count
	return center /= corners.size();
}

glm::mat4 ShadowMap::CalculateViewMatrix(const Vec4s& corners, const glm::vec3& lightDir)
{
	// Calculate center
	glm::vec3 center = CalculateCenter(corners);
	// Calculate view matrix (UP = 0, 1, 0)
	glm::mat4 matrix = glm::lookAt(center - lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));
	// Return
	return matrix;
}

glm::mat4 ShadowMap::CalculateProjMatrix(const Vec4s& corners, const glm::mat4& lightView)
{
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Set min and max to the min or max f32 value supported
	auto min = glm::vec3(std::numeric_limits<f32>::max());
	auto max = glm::vec3(std::numeric_limits<f32>::lowest());

	// For all corners
	for (const auto& corner : corners)
	{
		// Convert to light space
		auto cornerLS = glm::vec3(lightView * corner);
		// Check bounds
		min = glm::min(min, cornerLS);
		max = glm::max(max, cornerLS);
	}

	// Lambda to check min offset
	auto CheckMinOffset = [] (glm::vec3& min, f32 offset)
	{
		if (min.z < 0.0f)
		{
			min.z *= offset;
		}
		else
		{
			min.z /= offset;
		}
	};
	// Lambda to check max offset
	auto CheckMaxOffset = [] (glm::vec3& max, f32 offset)
	{
		if (max.z < 0.0f)
		{
			max.z /= offset;
		}
		else
		{
			max.z *= offset;
		}
	};

	// Check min and max offset
	CheckMinOffset(min, settings.shadows.offset);
	CheckMaxOffset(max, settings.shadows.offset);

	// Return an orthographic projection matrix of the frustum
	return glm::ortho
	(
		min.x, max.x,
		min.y, max.y,
		min.z, max.z
	);
}

Vec4s ShadowMap::CalculateFrustumCorners(const glm::mat4& proj, const glm::mat4& view)
{
	// Calculate the inverse of the projection-view matrix
	auto inverse = glm::inverse(proj * view);

	// Define frustum corners
	Vec4s frustumCorners;

	// For all x
	for (usize x = 0; x < 2; ++x)
	{
		// For all y
		for (usize y = 0; y < 2; ++y)
		{
			// For all z
			for (usize z = 0; z < 2; ++z)
			{
				// Calculate corner
				glm::vec4 corner = inverse * glm::vec4
				(
					2.0f * static_cast<f32>(x) - 1.0f,
					2.0f * static_cast<f32>(y) - 1.0f,
					2.0f * static_cast<f32>(z) - 1.0f,
					1.0f
				);
				// Perform perspective division
				frustumCorners.emplace_back(corner / corner.w);
			}
		}
	}

	// Return
	return frustumCorners;
}

void ShadowMap::BindShadowFBO() const
{
	// Bind
	buffer->Bind();
}

void ShadowMap::BindDefaultFBO() const
{
	// Unbind
	buffer->Unbind();
}