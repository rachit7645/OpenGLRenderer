#include "Plane.h"

// Using namespaces
using namespace Maths;

Plane::Plane(const glm::vec4& equation)
	: equation(equation),
      normal(equation.x, equation.y, equation.z),
      distance(equation.w)
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