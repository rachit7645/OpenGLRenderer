#include "Light.h"

using namespace Entities;

Light::Light(const glm::vec3& position, const glm::vec3& colour) : position(position), colour(colour) {}
Light::Light() : position(0.0f, 0.0f, 0.0f), colour(0.0f, 0.0f, 0.0f) {}