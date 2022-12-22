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

bool Frustum::IsOnFrustum(const Camera& camera, const Entity& entity)
{
	// Update planes
	Update(camera, entity);
	// For each mesh
	for (const auto& mesh : entity.model->meshes)
	{
		// Get global aabb
		auto aabb = GetGlobalAABB(mesh.aabb);
		// For each plane
		for (const auto& plane : planes)
		{
			// If AABB is on plane
			if (IsOnPlane(plane, aabb))
			{
				// Return true
				return true;
			}
		}
	}
	// If all checks fail
	LOG_DEBUG("{}\n", "Check failed!");
	return false;
}

void Frustum::Update(const Camera& camera, const Entity& entity)
{
	// Calculate model matrix
	m_model = Maths::CreateModelMatrix(entity.position, entity.rotation, entity.scale);

	// Calculate trigonometric ratios
	f32 halfVSide = FAR_PLANE * std::tan(glm::radians(FOV) * 0.5f);
	f32 halfHSide = halfVSide * ASPECT_RATIO;
	// Calculate multiplier
	glm::vec3 frontMultFar = FAR_PLANE * camera.GetForward();

	// Calculate top plane
	planes[PLANE_TOP] =
	{
	camera.position,
	glm::cross(camera.GetRight(), frontMultFar - camera.GetUp() * halfVSide)
	};

	// Calculate bottom plane
	planes[PLANE_BOTTOM] =
	{
	camera.position,
	glm::cross(frontMultFar + camera.GetUp() * halfVSide, camera.GetRight())
	};

	// Calculate left plane
	planes[PLANE_LEFT] =
	{
	camera.position,
	glm::cross(frontMultFar - camera.GetRight() * halfHSide, camera.GetUp())
	};

	// Calculate right plane
	planes[PLANE_RIGHT] =
	{
	camera.position,
	glm::cross(camera.GetUp(), frontMultFar + camera.GetRight() * halfHSide)
	};

	// Calculate near plane
	planes[PLANE_NEAR] =
	{
	camera.position + NEAR_PLANE * camera.GetForward(),
	camera.GetForward()
	};

	// Calculate far plane
	planes[PLANE_FAR] =
	{
		camera.position + frontMultFar,
		-camera.GetForward()
	};
}

AABB Frustum::GetGlobalAABB(const AABB& aabb)
{
	// Calculate global center
	auto globalCenter = m_model * glm::vec4(aabb.center, 1.0f);

	// Get matrix transforms
	glm::vec3 right   =  m_model[0] * aabb.extents.x;
	glm::vec3 up      =  m_model[1] * aabb.extents.y;
	glm::vec3 forward = -m_model[2] * aabb.extents.z;

	// Calculate extent X
	f32 newIi = std::abs(glm::dot(X, right)) +
				std::abs(glm::dot(X, up))    +
				std::abs(glm::dot(X, forward));

	// Calculate extent Y
	f32 newIj = std::abs(glm::dot(Y, right)) +
				std::abs(glm::dot(Y, up))    +
				std::abs(glm::dot(Y, forward));

	// Calculate extent Z
	f32 newIk = std::abs(glm::dot(Z, right)) +
				std::abs(glm::dot(Z, up))    +
				std::abs(glm::dot(Z, forward));

	// Return new AABB
	return AABB(globalCenter, newIi, newIj, newIk);
}

bool Frustum::IsOnPlane(const Plane& plane, const AABB& aabb)
{
	// Get projection interval radius
	f32 rX = aabb.extents.x * std::abs(plane.normal.x);
	f32 rY = aabb.extents.y * std::abs(plane.normal.y);
	f32 rZ = aabb.extents.z * std::abs(plane.normal.z);
	// Combine
	f32 r = rX + rY + rZ;
	// Perform check
	return -r <= GetDistance(plane, aabb.center);
}

// TODO: Move to plane
f32 Frustum::GetDistance(const Plane& plane, const glm::vec3& point)
{
	return glm::dot(plane.normal, point) - plane.distance;
}