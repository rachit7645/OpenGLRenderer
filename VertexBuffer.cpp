#include "VertexBuffer.h"

using namespace Renderer;

void VertexBuffer::CreateBuffer()
{
	glGenBuffers(1, &id);
}

void VertexBuffer::BufferData(GLenum type, const std::vector<GLfloat>& data)
{
	glBufferData
	(
		type,
		static_cast<GLsizeiptr>(data.size() * sizeof(GLfloat)),
		data.data(),
		GL_STATIC_DRAW
	);
}

void VertexBuffer::BufferData(GLenum type, const std::vector<GLuint>& data)
{
	glBufferData
	(
		type,
		static_cast<GLsizeiptr>(data.size() * sizeof(GLuint)),
		data.data(),
		GL_STATIC_DRAW
	);
}

void VertexBuffer::SetVertexAttribute
(
	GLuint index,
	GLint size,
	GLenum type,
	GLsizei stride,
	const void* pointer
)
{
	glVertexAttribPointer
	(
		index,
		size,
		type,
		GL_FALSE,
		stride,
		pointer
	);
}

void VertexBuffer::Bind(GLenum type) const
{
	glBindBuffer(type, id);
}

void VertexBuffer::Unbind(GLenum type) const
{
	glBindBuffer(type, 0);
}

VertexBuffer::~VertexBuffer()
{
	// ID must not be zero
	if (id)
	{
		glDeleteBuffers(1, &id);
	}
}