#ifndef VERTEX_H
#define VERTEX_H

#include "GLM.h"

namespace Renderer
{
	class Vertex
	{
	public:
		// Default constructor
		Vertex() = default;
		// Main Constructor
		Vertex
		(
			const glm::vec3& position,
			const glm::vec2& txCoord,
			const glm::vec3& normal,
			const glm::vec3& tangent
		);
		// Vertex position
		glm::vec3 position = {};
		// Texture coordinates
		glm::vec2 txCoord = {};
		// Vertex normal
		glm::vec3 normal = {};
		// Vertex tangent
		glm::vec3 tangent = {};
	};
}

#endif
