#include "DirectionalLight.h"

using namespace Entities;

DirectionalLight::DirectionalLight(const glm::vec3& position, const glm::vec3& color)
	: position(position, 1.0f),
	  color(color, 1.0f)
{
}