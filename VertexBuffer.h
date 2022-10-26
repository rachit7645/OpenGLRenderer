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
		// Destructor
		~VertexBuffer();

		void Bind(GLenum type)   const;
		void Unbind(GLenum type) const;

		GLuint id   = 0;
	protected:
		void CreateBuffer();

		void BufferData(GLenum type, const std::vector<GLfloat>& data);
		void BufferData(GLenum type, const std::vector<GLuint>& data);

		void SetVertexAttribute
		(
			GLuint index,
			GLint size,
			GLenum type,
			GLsizei stride,
			const void* pointer
		);
	public:
		friend class VertexArray;
	};
}

#endif