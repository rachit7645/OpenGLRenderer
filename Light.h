#ifndef LIGHT_H
#define LIGHT_H

#include "GLM.h"
#include "Util.h"

namespace Entities
{
	// TODO: Support multiple lights at once
	// TODO: Add a attenuation factor to the lights
	// TODO: Add an ImGui widget for live editing
	
	class Light
	{
	public:
		Light(const glm::vec3& position, const glm::vec3& colour) : position(position), colour(colour) {}
		glm::vec3 position;
		glm::vec3 colour;
	};
}


#endif