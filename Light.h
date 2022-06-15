#ifndef LIGHT_H
#define LIGHT_H

#include "GLM.h"
#include "Util.h"

namespace Entities
{
	// TODO: Add a attenuation factor to the lights
	// TODO: Add an ImGui widget for live editing

	class Light
	{
	public:
		Light(const glm::vec3& position, const glm::vec3& colour);
		glm::vec4 position;
		glm::vec4 colour;
	};
}


#endif