#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "Util.h"

namespace Renderer
{
	class VertexBuffer
	{
	public:
		// Default constructor
		VertexBuffer() = default;
		// Standard vbo
		VertexBuffer(GLuint slot, GLint coordSize, const std::vector<f32>& data);
		// Element buffer object
		explicit VertexBuffer(const std::vector<u32>& data);
		// Destructor
		~VertexBuffer();

		GLuint id = 0;
	};
}

#endif // VERTEX_BUFFER_H