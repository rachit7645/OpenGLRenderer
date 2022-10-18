#include "ShadowMap.h"

#include <limits>
#include <vector>

#include "Util.h"
#include "RenderConstants.h"
#include "Maths.h"
#include "FBOAttachment.h"

using namespace Renderer;

using Entities::Camera;

using Vec4s = ShadowMap::Vec4s;
using Mat4s = ShadowMap::Mat4s;

          glm::vec4  MAP_BORDER        = {1.0f, 1.0f, 1.0f, 1.0f};
constexpr glm::ivec2 SHADOW_DIMENSIONS = {2048, 2048};
constexpr f32        SHADOW_OFFSET     = 10.0f;

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
	Renderer::FBOAttachment depth =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_BORDER,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_DEPTH_ATTACHMENT,
		&MAP_BORDER[0]
	};

	buffer->width  = SHADOW_DIMENSIONS.x;
	buffer->height = SHADOW_DIMENSIONS.y;
	buffer->depth  = static_cast<GLsizei>(shadowLevels.size() + 1);

	buffer->CreateFrameBuffer();
	buffer->Bind();
	buffer->SetDrawBuffer(GL_NONE);
	buffer->SetReadBuffer(GL_NONE);
	buffer->AddArrayTexture(buffer->depthTexture, depth);
	buffer->CheckStatus();
	buffer->EnableDepth();
	buffer->Unbind();

	m_matrixBuffer->LoadDistances(shadowLevels);
}

void ShadowMap::Update(const Camera& camera, const glm::vec3& lightPos)
{
	auto matrices = CalculateLightSpaceMatrices(camera, glm::normalize(lightPos));
	m_matrixBuffer->LoadMatrices(matrices);
}

Mat4s ShadowMap::CalculateLightSpaceMatrices
(
	const Camera& camera,
	const glm::vec3& lightDir
)
{
	auto matrices = Mat4s(shadowLevels.size() + 1);

	matrices.front() = CalculateLightSpaceMatrix(NEAR_PLANE,          shadowLevels.front(), camera, lightDir);
	matrices.back()  = CalculateLightSpaceMatrix(shadowLevels.back(), FAR_PLANE,            camera, lightDir);

	for (usize i = 1; i < shadowLevels.size(); ++i)
	{
		matrices[i] = CalculateLightSpaceMatrix(shadowLevels[i - 1], shadowLevels[i], camera, lightDir);
	}

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
	auto proj = glm::perspective
	(
		glm::radians(FOV),
		ASPECT_RATIO,
		nearPlane,
		farPlane
	);

	auto corners = CalculateFrustumCorners(proj, Maths::CreateViewMatrix(camera));

	auto lightView = CalculateViewMatrix(corners, lightDir);
	auto lightProj = CalculateProjMatrix(corners, lightView);

	return lightProj * lightView;
}

glm::vec3 ShadowMap::CalculateCenter(const Vec4s& corners)
{
	auto center = glm::vec3(0.0f);

	for (const auto& corner : corners)
	{
		center += glm::vec3(corner);
	}

	return center /= corners.size();
}

glm::mat4 ShadowMap::CalculateViewMatrix(const Vec4s& corners, const glm::vec3& lightDir)
{
	auto center = CalculateCenter(corners);
	auto matrix = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));
	return matrix;
}

glm::mat4 ShadowMap::CalculateProjMatrix(const Vec4s& corners, const glm::mat4& lightView)
{
	auto min = glm::vec3(std::numeric_limits<f32>::max());
	auto max = glm::vec3(std::numeric_limits<f32>::min());

	for (const auto& corner : corners)
	{
		auto cornerLS = lightView * corner;
		min.x = std::min(min.x, cornerLS.x);
		max.x = std::max(max.x, cornerLS.x);
		min.y = std::min(min.y, cornerLS.y);
		max.y = std::max(max.y, cornerLS.y);
		min.z = std::min(min.z, cornerLS.z);
		max.z = std::max(max.z, cornerLS.z);
	}

	if (min.z < 0.0f)
	{
		min.z *= SHADOW_OFFSET;
	}
	else
	{
		min.z /= SHADOW_OFFSET;
	}

	if (max.z < 0.0f)
	{
		max.z /= SHADOW_OFFSET;
	}
	else
	{
		max.z *= SHADOW_OFFSET;
	}

	return glm::ortho
	(
		min.x, max.x,
		min.y, max.y,
		min.z, max.z
	);
}

Vec4s ShadowMap::CalculateFrustumCorners(const glm::mat4& proj, const glm::mat4& view)
{
	auto inverse = glm::inverse(proj * view);

	Vec4s frustumCorners;

	for (usize x = 0; x < 2; ++x)
	{
		for (usize y = 0; y < 2; ++y)
		{
			for (usize z = 0; z < 2; ++z)
			{
				glm::vec4 corner = inverse * glm::vec4
				(
					2.0f * static_cast<f32>(x) - 1.0f,
					2.0f * static_cast<f32>(y) - 1.0f,
					2.0f * static_cast<f32>(z) - 1.0f,
					1.0f
				);

				frustumCorners.emplace_back(corner / corner.w);
			}
		}
	}

	return frustumCorners;
}

void ShadowMap::BindShadowFBO() const
{
	buffer->Bind();
}

void ShadowMap::BindDefaultFBO() const
{
	buffer->Unbind();
}