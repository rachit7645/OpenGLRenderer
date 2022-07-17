#ifndef LIGHT_H
#define LIGHT_H

#include "GLM.h"

namespace Entities
{
	class Light
	{
	public:
		// Default constructor
		Light();
		// Main constructor
		Light
		(
			const glm::vec3& position,
			const glm::vec3& ambient,
			const glm::vec3& diffuse,
			const glm::vec3& specular,
			const glm::vec3& attenuation
		);

		// Light data (must match struct in shaders)
		alignas(16) glm::vec4 position;
		alignas(16) glm::vec4 ambient;
		alignas(16) glm::vec4 diffuse;
		alignas(16) glm::vec4 specular;
		alignas(16) glm::vec4 attenuation;
	};
}


#endif