#include "UniformBuffer.h"

#include "Util.h"

using namespace Renderer;

UniformBuffer::UniformBuffer(GLuint slot, GLsizeiptr size) : size(size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, id);
}

UniformBuffer::UniformBuffer() : id(0), size(0) {}

UniformBuffer::~UniformBuffer()
{
	// ID must not be zero
	if (id)
	{
		glDeleteBuffers(1, &id);
	}
}