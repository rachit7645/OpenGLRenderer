#include "Plane.h"

using namespace Maths;

Plane::Plane(const glm::vec3& point, const glm::vec3& normal)
	: normal(glm::normalize(normal)),
	  distance(glm::dot(normal, point))
{
}