#include "Light.h"

using namespace Entities;

Light::Light() : position(0.0f, 0.0f, 0.0f, 1.0f), colour(0.0f, 0.0f, 0.0f, 1.0f), attenuation(1.0f, 0.0f, 0.0f, 1.0f) {}

Light::Light(const glm::vec3& position, const glm::vec3& colour)
	: position(position, 1.0f), colour(colour, 1.0f), attenuation(1.0f, 0.0f, 0.0f, 1.0f) {}

Light::Light(const glm::vec3& position, const glm::vec3& colour, const glm::vec3& attenuation)
	: position(position, 1.0f), colour(colour, 1.0f), attenuation(attenuation, 1.0f) {}