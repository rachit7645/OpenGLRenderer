#include "Frustum.h"

#include <algorithm>

#include "RenderConstants.h"
#include "Maths.h"

// Using namespaces
using namespace Maths;

// Usings
using Entities::Entity;
using Entities::Camera;
using Maths::AABB;
using Maths::Plane;

// Constant direction vectors
constexpr glm::vec3 X = {1.0f, 0.0f, 0.0f};
constexpr glm::vec3 Y = {0.0f, 1.0f, 0.0f};
constexpr glm::vec3 Z = {0.0f, 0.0f, 1.0f};

// TODO: Only update frustum if camera changes

Frustum::Frustum()
{
	// Create projection matrix
	m_projection = glm::perspective
	(
		glm::radians(Renderer::FOV),
		Renderer::ASPECT_RATIO,
		Renderer::NEAR_PLANE,
		Renderer::FAR_PLANE
	);
}

bool Frustum::IsVisible(const Entity& entity, const Camera& camera)
{
	// Get meshes
	const auto& meshes = entity.model->meshes;
	// Update frustum
	Update(entity, camera);

	// Return true if any meshes are visible
	return std::any_of(meshes.begin(), meshes.end(), [this] (const auto& mesh)
	{
		// Return true if mesh is visible
		return IsOnFrustum(GetGlobalAABB(mesh.aabb));
	});
}

bool Frustum::IsOnFrustum(const AABB& aabb) const
{
	// Return true if aabb is on any plane
	return std::any_of(m_planes.begin(), m_planes.end(), [aabb, this] (const auto& plane)
	{
		// Check if AABB is on plane
		return IsOnPlane(plane, aabb);
	});
}

// FIXME: This is broken for large objects
bool Frustum::IsOnPlane(const Plane& plane, const AABB& aabb) const
{
	// Get aabb parameters
	const auto& minp = aabb.min;
	const auto& maxp = aabb.max;
	// Get plane equation
	auto equation = glm::vec4(plane.normal, plane.distance);

	// Lambda function to check point
	auto CheckPoint = [equation] (f32 x, f32 y, f32 z)
	{
		// Check point
		return glm::dot(equation, glm::vec4(x, y, z, 1.0f)) < 0.0f;
	};

	// Check all points of the AABB
	if
	(
		CheckPoint(minp.x, minp.y, minp.z) &&
		CheckPoint(maxp.x, minp.y, minp.z) &&
		CheckPoint(minp.x, maxp.y, minp.z) &&
		CheckPoint(maxp.x, maxp.y, minp.z) &&
		CheckPoint(minp.x, minp.y, maxp.z) &&
		CheckPoint(maxp.x, minp.y, maxp.z) &&
		CheckPoint(minp.x, maxp.y, maxp.z) &&
		CheckPoint(maxp.x, maxp.y, maxp.z)
	)
	{
		// Return false if all checks fail
		return false;
	}

	// Return true if any checks pass
	return true;
}

void Frustum::Update(const Entity& entity, const Camera& camera)
{
	// Create model matrix
	m_model = Maths::CreateModelMatrix(entity.position, entity.rotation, entity.scale);
	// Create view matrix
	m_view = Maths::CreateViewMatrix(camera);
	// Combine view and projection matrix
	auto matrix = m_projection * m_view * m_model;

	// Calculate left plane
	m_planes[PLANE_LEFT] = Maths::Plane
	(
		matrix[3][0] + matrix[0][0],
		matrix[3][1] + matrix[0][1],
		matrix[3][2] + matrix[0][2],
		matrix[3][3] + matrix[0][3]
	);

	// Calculate right plane
	m_planes[PLANE_RIGHT] = Maths::Plane
	(
		matrix[3][0] - matrix[0][0],
		matrix[3][1] - matrix[0][1],
		matrix[3][2] - matrix[0][2],
		matrix[3][3] - matrix[0][3]
	);

	// Calculate bottom plane
	m_planes[PLANE_BOTTOM] = Maths::Plane
	(
		matrix[3][0] + matrix[1][0],
		matrix[3][1] + matrix[1][1],
		matrix[3][2] + matrix[1][2],
		matrix[3][3] + matrix[1][3]
	);

	// Calculate top plane
	m_planes[PLANE_TOP] = Maths::Plane
	(
		matrix[3][0] - matrix[1][0],
		matrix[3][1] - matrix[1][1],
		matrix[3][2] - matrix[1][2],
		matrix[3][3] - matrix[1][3]
	);

	// Calculate near plane
	m_planes[PLANE_NEAR] = Maths::Plane
	(
		matrix[3][0] + matrix[2][0],
		matrix[3][1] + matrix[2][1],
		matrix[3][2] + matrix[2][2],
		matrix[3][3] + matrix[2][3]
	);

	// Calculate far plane
	m_planes[PLANE_FAR] = Maths::Plane
	(
		matrix[3][0] - matrix[2][0],
		matrix[3][1] - matrix[2][1],
		matrix[3][2] - matrix[2][2],
		matrix[3][3] - matrix[2][3]
	);
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