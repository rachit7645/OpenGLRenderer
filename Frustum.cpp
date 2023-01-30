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

// TODO:  Only update frustum if camera changes
// FIXME: Does not work for large objects

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
		return IsOnFrustum(mesh.aabb.Transform(m_model));
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

bool Frustum::IsOnPlane(const Plane& plane, const AABB& aabb) const
{
    // Check positive
    if (plane.GetDistance(aabb.GetPositive(plane.normal)) < 0.0f)
    {
        // AABB is not on plane
        return false;
    }

    // Check negative
    if (plane.GetDistance(aabb.GetNegative(plane.normal)) < 0.0f)
    {
        // AABB is not on plane
        return false;
    }

    // AABB is on plane
    return true;
}

void Frustum::Update(const Entity& entity, const Camera& camera)
{
	// Create model matrix
	m_model = Maths::CreateModelMatrix(entity.position, entity.rotation, entity.scale);
	// Create view matrix
	m_view = Maths::CreateViewMatrix(camera);

	// Combine matrices
	auto matrix = m_projection * m_view * m_model;

	// Calculate left plane
	m_planes[PLANE_LEFT] = Maths::Plane
	(
		 matrix[0][3] + matrix[0][0],
         matrix[1][3] + matrix[1][0],
         matrix[2][3] + matrix[2][0],
        -matrix[3][3] - matrix[3][0]
	);

	// Calculate right plane
	m_planes[PLANE_RIGHT] = Maths::Plane
	(
         matrix[0][3] - matrix[0][0],
         matrix[1][3] - matrix[1][0],
         matrix[2][3] - matrix[2][0],
        -matrix[3][3] + matrix[3][0]
	);

	// Calculate bottom plane
	m_planes[PLANE_BOTTOM] = Maths::Plane
	(
         matrix[0][3] + matrix[0][1],
         matrix[1][3] + matrix[1][1],
         matrix[2][3] + matrix[2][1],
        -matrix[3][3] - matrix[3][1]
	);

	// Calculate top plane
	m_planes[PLANE_TOP] = Maths::Plane
	(
         matrix[0][3] - matrix[0][1],
         matrix[1][3] - matrix[1][1],
         matrix[2][3] - matrix[2][1],
        -matrix[3][3] + matrix[3][1]
	);

	// Calculate near plane
	m_planes[PLANE_NEAR] = Maths::Plane
	(
         matrix[0][3] + matrix[0][2],
         matrix[1][3] + matrix[1][2],
         matrix[2][3] + matrix[2][2],
        -matrix[3][3] - matrix[3][2]
	);

	// Calculate far plane
	m_planes[PLANE_FAR] = Maths::Plane
	(
         matrix[0][3] - matrix[0][2],
         matrix[1][3] - matrix[1][2],
         matrix[2][3] - matrix[2][2],
        -matrix[3][3] + matrix[3][2]
	);
}