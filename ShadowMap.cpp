#include "ShadowMap.h"

#include "Util.h"
#include "RenderConstants.h"
#include "Maths.h"

using namespace Renderer;

using Entities::Camera;

using Vec4s = ShadowMap::Vec4s;
using Mat4s = ShadowMap::Mat4s;

constexpr glm::ivec2 SHADOW_DIMENSIONS = {1024, 1024};

constexpr std::array<f32, 4> shadowLevels =
{
	FAR_PLANE / 50.0f,
	FAR_PLANE / 25.0f,
	FAR_PLANE / 10.0f,
	FAR_PLANE / 2.0f
};

ShadowMap::ShadowMap()
	: buffer(std::make_shared<FrameBuffer>(SHADOW_DIMENSIONS.x, SHADOW_DIMENSIONS.y, shadowLevels.size() + 1))
{
}

void ShadowMap::Update(const Camera& camera, const glm::vec3& lightPos)
{
	auto matrices = CalculateLightSpaceMatrices(camera, glm::normalize(lightPos));
}

Mat4s ShadowMap::CalculateLightSpaceMatrices
(
	const Camera& camera,
	const glm::vec3& lightDir
)
{
	Mat4s matrices;

	for (usize i = 0; i < shadowLevels.size() + 1; ++i)
	{
		if (i == 0)
		{
			matrices.emplace_back(CalculateLightSpaceMatrix(NEAR_PLANE, shadowLevels[i], camera, lightDir));
		}
		else if (i < shadowLevels.size())
		{
			matrices.emplace_back(CalculateLightSpaceMatrix(shadowLevels[i - 1], shadowLevels[i], camera, lightDir));
		}
		else
		{
			matrices.emplace_back(CalculateLightSpaceMatrix(shadowLevels[i - 1], FAR_PLANE, camera, lightDir));
		}
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
		glm::radians(camera.distance),
		ASPECT_RATIO,
		nearPlane,
		farPlane
	);

	auto corners = CalculateFrustumCorners(proj, Maths::CreateViewMatrix(camera));

	auto center = glm::vec3(0.0f);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

	f32 minX, minY, minZ;
	f32 maxX, maxY, maxZ;

	minX = minY = minZ = std::numeric_limits<f32>::max();
	maxX = maxY = maxZ = std::numeric_limits<f32>::min();

	for (const auto& v : corners)
	{
		auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr f32 zMult = 10.0f;

	if (minZ < 0.0f)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}

	if (maxZ < 0.0f)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	auto lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	return lightProjection * lightView;
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
