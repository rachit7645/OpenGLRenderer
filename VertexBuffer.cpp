#include "VertexBuffer.h"

using namespace Renderer;

// TODO: Add pipeline style functions for flecibility

VertexBuffer::VertexBuffer(GLuint slot, GLint coordSize, const std::vector<f32>& data)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);

	glBufferData
	(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(data.size() * sizeof(GLfloat)),
		data.data(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer
	(
		slot,
		coordSize,
		GL_FLOAT,
		GL_FALSE,
		0,
		nullptr
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::VertexBuffer(const std::vector<u32>& data)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(data.size() * sizeof(GLuint)),
		data.data(),
		GL_STATIC_DRAW
	);
}

VertexBuffer::~VertexBuffer()
{
	// ID must not be zero
	if (id)
	{
		glDeleteBuffers(1, &id);
	}
}