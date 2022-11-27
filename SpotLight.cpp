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

glm::vec2 SpotLight::GetCutOff()
{
	auto degCutOff = glm::vec2(cutOff.x, cutOff.y);
	// Convert to radians
	degCutOff = glm::acos(degCutOff);
	// Convert to degrees
	degCutOff = glm::degrees(degCutOff);
	// Return
	return degCutOff;
}

void SpotLight::SetCutOff(const glm::vec2& newCutOff)
{
	// Convert to radians
	auto cosCutOff = glm::radians(newCutOff);
	// Convert to cosine
	cosCutOff = glm::cos(cosCutOff);
	// Set value
	cutOff = glm::vec4(cosCutOff, 1.0f, 1.0f);
}