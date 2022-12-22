#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Camera.h"
#include "Util.h"
#include "Entity.h"
#include "AABB.h"
#include "Plane.h"

namespace Maths
{
	class Frustum
	{
	public:
		// Default constructor
		Frustum() = default;

		// Check for intersection
		bool IsOnFrustum(const Entities::Camera& camera, const Entities::Entity& entity);

		// Planes
		std::array<Maths::Plane, 6> planes;
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
		// Update Planes
		void Update(const Entities::Camera& camera, const Entities::Entity& entity);
		// Get global AABB
		AABB GetGlobalAABB(const AABB& aabb);
		// Check if AABB is on plane
		bool IsOnPlane(const Maths::Plane& plane, const AABB& aabb);
		// Get distance between a plane and a point
		f32 GetDistance(const Maths::Plane& plane, const glm::vec3& point);

		// Model
		glm::mat4 m_model;
	};
}

#endif
