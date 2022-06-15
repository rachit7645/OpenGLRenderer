#include "Light.h"

using namespace Entities;

Light::Light(const glm::vec3& position, const glm::vec3& colour) : position(position, 1.0f), colour(colour, 1.0f) {}