#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <unordered_map>
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "Util.h"

namespace Renderer
{
	class VertexArray
	{
	public:
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
		// Default constructor
		VertexArray();
		// Destructor
		~VertexArray();

		GLuint id;
		GLsizei vertexCount;
		std::unordered_map<const char*, std::shared_ptr<VertexBuffer>> buffers;
	};
}

#endif // VERTEX_ARRAY_H