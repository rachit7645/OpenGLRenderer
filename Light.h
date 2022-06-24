#ifndef LIGHT_H
#define LIGHT_H

#include "GLM.h"
#include "Util.h"

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
		glm::vec4 position;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 attenuation;
	};
}


#endif