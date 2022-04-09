#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Util.h"

namespace Entities
{
	struct Light
	{
		Light(const glm::vec3& p_position, const glm::vec3& p_colour) : position(p_position), colour(p_colour) {}
		glm::vec3 position;
		glm::vec3 colour;
	};
}


#endif