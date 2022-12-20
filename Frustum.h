#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Camera.h"
#include "Util.h"
#include "Entity.h"
#include "AABB.h"

namespace Maths
{
	class Frustum
	{
	public:
		// Default constructor
		Frustum() = default;

		// Update frustum
		void Update(const Entities::Camera& camera);
		// Check for intersection
		bool IsOnFrustum(const Entities::Entity& entity);

		// Planes
		std::array<glm::vec4, 6> planes;
		// Plane IDs
		enum PlaneIDs : usize
		{
			PLANE_TOP    = 0,
			PLANE_BOTTOM = 1,
			PLANE_LEFT   = 2,
			PLANE_RIGHT  = 3,
			PLANE_NEAR   = 4,
			PLANE_FAR    = 5
		};
	private:
		// Get global AABB
		AABB GetGlobalAABB(const Entities::Entity& entity, const AABB& aabb);
		// Check if AABB is on plane
		bool IsOnPlane(const glm::vec4& plane, const AABB& aabb);
		// Get distance between a plane and a point
		f32 GetDistance(const glm::vec4& plane, const glm::vec3 point);

		// Projection
		glm::mat4 m_projection;
		// View
		glm::mat4 m_view;
	};
}

#endif
