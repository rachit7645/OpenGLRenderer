#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <array>

#include "GLM.h"
#include "AABB.h"
#include "Plane.h"
#include "Util.h"
#include "Camera.h"
#include "Entity.h"

namespace Maths
{
	class Frustum
	{
	public:
		// Main constructor
		Frustum();
		// Check if entity is visible
		bool IsVisible(const Entities::Entity& entity, const Entities::Camera& camera);
	private:
		// Check if AABB is inside frustum
		bool IsInFrustum(const Maths::AABB& aabb) const;
        // Check if corner is in plane
        bool IsCornerNotInPlane(usize corner, const Maths::Plane& plane, const Maths::AABB& aabb) const;
		// Update frustum bounds
		void Update(const Entities::Entity& entity, const Entities::Camera& camera);

		// Frustum Planes
		std::array<Maths::Plane, 6> m_planes = {};

		// Plane IDs
		enum Planes : usize
		{
			PLANE_LEFT   = 0,
			PLANE_RIGHT  = 1,
			PLANE_BOTTOM = 2,
			PLANE_TOP    = 3,
			PLANE_NEAR   = 4,
			PLANE_FAR    = 5
		};

		// Projection matrix
		glm::mat4 m_projection = {};
		// View matrix
		glm::mat4 m_view = {};
		// Model matrix
		glm::mat4 m_model = {};
	};
}

#endif
