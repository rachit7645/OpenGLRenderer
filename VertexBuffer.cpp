#include "VertexBuffer.h"

using namespace Renderer;

void VertexBuffer::CreateBuffer()
{
	// Generate buffer
	glGenBuffers(1, &id);
}

void VertexBuffer::BufferData(GLenum type, const std::vector<Vertex>& data)
{
	// Buffer packed data
	glBufferData
	(
		type,
		static_cast<GLsizeiptr>(data.size() * sizeof(Vertex)),
		data.data(),
		GL_STATIC_DRAW
	);
}

void VertexBuffer::BufferData(GLenum type, const std::vector<GLfloat>& data)
{
	// Buffer f32 data
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
	// Buffer u32 data
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
	// Set vertex attribute
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
	// Bind
	glBindBuffer(type, id);
}

void VertexBuffer::Unbind(GLenum type) const
{
	// Unbind
	glBindBuffer(type, 0);
}

VertexBuffer::~VertexBuffer()
{
	// ID must not be zero
	if (id)
	{
		// Clear buffer
		glDeleteBuffers(1, &id);
	}
}