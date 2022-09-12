#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "Util.h"

namespace Renderer
{
	class VertexArray
	{
	public:
		// Default constructor
		VertexArray() = default;
		// Init default VAO
		VertexArray
		(
			const std::vector<f32>& vertices,
			const std::vector<u32>& indices,
			const std::vector<f32>& txCoords,
			const std::vector<f32>& normals
		);
		// Init VAO for glDrawArrays
		VertexArray(GLuint coordSize, const std::vector<f32>& vertices);
		// Destructor
		~VertexArray();

		GLuint  id          = 0;
		GLsizei vertexCount = 0;

		std::unordered_map<std::string_view, std::shared_ptr<VertexBuffer>> buffers;
	};
}

#endif