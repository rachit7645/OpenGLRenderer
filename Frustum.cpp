#include "Frustum.h"

#include "RenderConstants.h"
#include "Maths.h"
#include "Log.h"

using namespace Maths;

using Entities::Camera;
using Entities::Entity;
using Renderer::FOV;
using Renderer::ASPECT_RATIO;
using Renderer::NEAR_PLANE;
using Renderer::FAR_PLANE;

// Constant direction vectors
constexpr glm::vec3 X = {1.0f, 0.0f, 0.0f};
constexpr glm::vec3 Y = {0.0f, 1.0f, 0.0f};
constexpr glm::vec3 Z = {0.0f, 0.0f, 1.0f};

void Frustum::Update(const Camera& camera)
{
	// Calculate projection matrix
	m_projection = glm::perspective
	(
		glm::radians(FOV),
		ASPECT_RATIO,
		NEAR_PLANE,
		FAR_PLANE
	);

	// Calculate view matrix
	m_view = Maths::CreateViewMatrix(camera);

	// Combine
	auto matrix = m_projection * m_view;

	// Calculate top plane
	planes[PLANE_TOP].x = matrix[3][0] - matrix[1][0];
	planes[PLANE_TOP].y = matrix[3][1] - matrix[1][1];
	planes[PLANE_TOP].z = matrix[3][2] - matrix[1][2];
	planes[PLANE_TOP].w = matrix[3][3] - matrix[1][3];

	// Calculate bottom plane
	planes[PLANE_BOTTOM].x = matrix[3][0] + matrix[1][0];
	planes[PLANE_BOTTOM].y = matrix[3][1] + matrix[1][1];
	planes[PLANE_BOTTOM].z = matrix[3][2] + matrix[1][2];
	planes[PLANE_BOTTOM].w = matrix[3][3] + matrix[1][3];

	// Calculate left plane
	planes[PLANE_LEFT].x = matrix[3][0] + matrix[0][0];
	planes[PLANE_LEFT].y = matrix[3][1] + matrix[0][1];
	planes[PLANE_LEFT].z = matrix[3][2] + matrix[0][2];
	planes[PLANE_LEFT].w = matrix[3][3] + matrix[0][3];

	// Calculate right plane
	planes[PLANE_RIGHT].x = matrix[3][0] - matrix[0][0];
	planes[PLANE_RIGHT].y = matrix[3][1] - matrix[0][1];
	planes[PLANE_RIGHT].z = matrix[3][2] - matrix[0][2];
	planes[PLANE_RIGHT].w = matrix[3][3] - matrix[0][3];

	// Calculate near plane
	planes[PLANE_NEAR].x = matrix[3][0] + matrix[2][0];
	planes[PLANE_NEAR].y = matrix[3][1] + matrix[2][1];
	planes[PLANE_NEAR].z = matrix[3][2] + matrix[2][2];
	planes[PLANE_NEAR].w = matrix[3][3] + matrix[2][3];

	// Calculate far plane
	planes[PLANE_FAR].x = matrix[3][0] - matrix[2][0];
	planes[PLANE_FAR].y = matrix[3][1] - matrix[2][1];
	planes[PLANE_FAR].z = matrix[3][2] - matrix[2][2];
	planes[PLANE_FAR].w = matrix[3][3] - matrix[2][3];
}

bool Frustum::IsOnFrustum(const Entity& entity)
{
	// For each mesh
	for (const auto& mesh : entity.model->meshes)
	{
		// Get global aabb
		auto aabb = GetGlobalAABB(entity, mesh.aabb);
		// For each frustum
		for (const auto& plane : planes)
		{
			if (IsOnPlane(plane, aabb))
			{
				return true;
			}
		}
	}
	// By default
	return false;
}

AABB Frustum::GetGlobalAABB(const Entity& entity, const AABB& aabb)
{
	// Get model matrix
	auto modelMatrix = Maths::CreateModelMatrix(entity.position, entity.rotation, entity.scale);
	// Calculate global center
	auto globalCenter = modelMatrix * glm::vec4(aabb.center, 1.0f);

	// Get matrix transforms
	glm::vec3 right   =  modelMatrix[0] * aabb.extents.x;
	glm::vec3 up      =  modelMatrix[1] * aabb.extents.y;
	glm::vec3 forward = -modelMatrix[2] * aabb.extents.z;

	// Calculate extents
	f32 newIi = std::abs(glm::dot(X, right)) + std::abs(glm::dot(X, up)) + std::abs(glm::dot(X, forward));
	f32 newIj = std::abs(glm::dot(Y, right)) + std::abs(glm::dot(Y, up)) + std::abs(glm::dot(Y, forward));
	f32 newIk = std::abs(glm::dot(Z, right)) + std::abs(glm::dot(Z, up)) + std::abs(glm::dot(Z, forward));

	return AABB(globalCenter, newIi, newIj, newIk);
}

bool Frustum::IsOnPlane(const glm::vec4& plane, const AABB& aabb)
{
	// Get projection interval radius
	f32 rX = aabb.extents.x * std::abs(plane.x);
	f32 rY = aabb.extents.y * std::abs(plane.y);
	f32 rZ = aabb.extents.z * std::abs(plane.z);
	// Combine
	f32 r = rX + rY + rZ;
	// Perform check
	return -r <= GetDistance(plane, aabb.center);
}

f32 Frustum::GetDistance(const glm::vec4& plane, const glm::vec3 point)
{
	return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
}