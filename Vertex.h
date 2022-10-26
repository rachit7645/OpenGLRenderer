#ifndef VERTEX_H
#define VERTEX_H

#include "GLM.h"

namespace Renderer
{
	class Vertex
	{
	public:
		Vertex(const glm::vec3& position, const glm::vec2& txCoord, const glm::vec3& normal);

		glm::vec3 position;
		glm::vec2 txCoord;
		glm::vec3 normal;
	};
}

#endif
