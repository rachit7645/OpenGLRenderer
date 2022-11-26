#include "SpotLight.h"

using namespace Entities;

SpotLight::SpotLight
(
	const glm::vec3& position,
	const glm::vec3& color,
	const glm::vec3& intensity,
	const glm::vec3& attenuation,
	const glm::vec3& direction,
	const glm::vec2& cutOff
)
	: position(position, 1.0f),
	  color(color, 1.0f),
	  intensity(intensity, 1.0f),
	  attenuation(attenuation, 1.0f),
	  direction(direction, 1.0f),
	  cutOff(glm::cos(glm::radians(cutOff)), 1.0f, 1.0f)
{
}