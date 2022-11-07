#ifndef LIGHT_H
#define LIGHT_H

#include "GLM.h"

namespace Entities
{
	class Light
	{
	public:
		// Default constructor
		Light() = default;
		// Main constructor
		Light
		(
			const glm::vec3& position,
			const glm::vec3& ambient,
			const glm::vec3& diffuse,
			const glm::vec3& attenuation
		);

		// Light data (must match struct in shaders)
		alignas(16) glm::vec4 position    = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 ambient     = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 diffuse     = {0.0f, 0.0f, 0.0f, 1.0f};
		alignas(16) glm::vec4 attenuation = {1.0f, 0.0f, 0.0f, 1.0f};
	};
}


#endif