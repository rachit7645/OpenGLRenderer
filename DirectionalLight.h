#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "GLM.h"

namespace Entities
{
	class DirectionalLight
	{
	public:
		// Default constructor
		DirectionalLight() = default;
		// Main constructor
		DirectionalLight
		(
			const glm::vec3& position,
			const glm::vec3& color,
			const glm::vec3& intensity
		);
		// Data
		alignas(16) glm::vec4 position  = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 color     = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 intensity = {0.0f, 0.0f, 0.0f, 1.0f};
	};
}

#endif
