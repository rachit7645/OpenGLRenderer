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
		explicit Plane(f32 x, f32 y, f32 z, f32 w);

		// Normalise plane equation
		void NormalizePlane();
		// Get distance from point
		f32 GetDistance(const glm::vec3& point) const;

		// Normal vector
		glm::vec3 normal = {};
		// Distance from origin
		f32 distance = {};
	};
}

#endif