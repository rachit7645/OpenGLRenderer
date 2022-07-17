#include "Light.h"

#include "Util.h"

using namespace Entities;

Light::Light()
	: position(0.0f, 0.0f, 0.0f, 1.0f),
	  ambient(0.0f, 0.0f, 0.0f, 1.0f),
	  diffuse(0.0f, 0.0f, 0.0f, 1.0f),
	  specular(0.0f, 0.0f, 0.0f, 1.0f),
	  attenuation(1.0f, 0.0f, 0.0f, 1.0f) {}

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
	 attenuation(attenuation, 1.0f) {}