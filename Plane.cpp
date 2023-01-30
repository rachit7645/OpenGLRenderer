#include "Plane.h"

// Using namespaces
using namespace Maths;

Plane::Plane(f32 x, f32 y, f32 z, f32 w)
	: normal(x, y, z),
      distance(w)
{
	// Normalize plane equation
	NormalizePlane();
}

void Plane::NormalizePlane()
{
	// Calculate magnitude
	f32 magnitude = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	// Normalize normal
	normal /= magnitude;
	// Normalize distance
	distance /= magnitude;
}

f32 Plane::GetDistance(const glm::vec3& point) const
{
	// Return distance
	return glm::dot(normal, point) - distance;
}