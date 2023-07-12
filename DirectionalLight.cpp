#include "DirectionalLight.h"

using namespace Entities;

DirectionalLight::DirectionalLight
(
    const glm::vec3& position,
    const glm::vec3& color,
    const glm::vec3& intensity
)
    : position(position, 1.0f),
      color(color, 1.0f),
      intensity(intensity, 1.0f)
{
}