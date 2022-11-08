#include "Light.h"

using namespace Entities;

Light::Light
(
	const glm::vec3& position,
	const glm::vec3& color,
	const glm::vec3& attenuation
)
	: position(position, 1.0f),
	  color(color, 1.0f),
	  attenuation(attenuation, 1.0f)
{
}