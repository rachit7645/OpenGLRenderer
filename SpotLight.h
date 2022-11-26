#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

namespace Entities
{
	class SpotLight
	{
	public:
		// Default constructor
		SpotLight() = default;
		// Main constructor
		SpotLight
		(
			const glm::vec3& position,
			const glm::vec3& color,
			const glm::vec3& intensity,
			const glm::vec3& attenuation,
			const glm::vec3& direction,
			const glm::vec2& cutOff
		);
		// Data
		alignas(16) glm::vec4 position    = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 color       = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 intensity   = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 attenuation = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 direction   = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 cutOff      = {0.0f, 0.0f, 1.0f, 1.0f};
	};
}

#endif