#ifndef PLANE_H
#define PLANE_H

#include "GLM.h"
#include "Util.h"

namespace Maths
{
	class Plane
	{
	public:
		// Default constructor
		Plane() = default;
		// Main constructor
		Plane(const glm::vec3& point, const glm::vec3& normal);
		// Normal vector
		glm::vec3 normal = {};
		// Distance from origin
		f32 distance = {};
	};
}

#endif
