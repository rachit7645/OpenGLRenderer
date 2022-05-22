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

		VertexBuffer(GLuint slot, GLuint coordSize, const std::vector<f32>& data);
		VertexBuffer(const std::vector<u32>& data);
		VertexBuffer();
		~VertexBuffer();
	};
}

#endif // VERTEX_BUFFER_H