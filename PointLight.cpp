#include "PointLight.h"

using namespace Entities;

PointLight::PointLight
(
	const glm::vec3& position,
	const glm::vec3& color,
	const glm::vec3& intensity,
	const glm::vec3& attenuation
)
	: position(position, 1.0f),
	  color(color, 1.0f),
	  intensity(intensity, 1.0f),
	  attenuation(attenuation, 1.0f)
{
}