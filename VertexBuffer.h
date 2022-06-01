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
		GLuint id;

		// Standard vbo
		VertexBuffer(GLuint slot, GLuint coordSize, const std::vector<f32>& data);
		// Element buffer object
		VertexBuffer(const std::vector<u32>& data);
		// Default constructor
		VertexBuffer();
		// Destructor
		~VertexBuffer();
	};
}

#endif // VERTEX_BUFFER_H