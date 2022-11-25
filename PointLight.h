#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "GLM.h"

namespace Entities
{
	class PointLight
	{
	public:
		// Default constructor
		PointLight() = default;
		// Main constructor
		PointLight
		(
			const glm::vec3& position,
			const glm::vec3& color,
			const glm::vec3& intensity,
			const glm::vec3& attenuation
		);
		// Light data (must match struct in shaders)
		alignas(16) glm::vec4 position    = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 color       = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 intensity   = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 attenuation = {0.0f, 0.0f, 0.0f, 1.0f};
	};
}


#endif