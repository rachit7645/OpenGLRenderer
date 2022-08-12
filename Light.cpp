#include "Light.h"

using namespace Entities;

Light::Light
(
	const glm::vec3& position,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	const glm::vec3& attenuation
)
	: position(position, 1.0f),
	  ambient(ambient, 1.0f),
	  diffuse(diffuse, 1.0f),
	  specular(specular, 1.0f),
	  attenuation(attenuation, 1.0f)
{
}