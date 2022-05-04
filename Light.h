#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Util.h"

namespace Entities
{
	class Light
	{
	public:
		Light(const glm::vec3 &position, const glm::vec3 &colour) : position{ position }, colour{ colour } {}
		glm::vec3 position;
		glm::vec3 colour;
	};
}


#endif