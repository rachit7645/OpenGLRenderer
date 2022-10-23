#include "UniformBuffer.h"

using namespace Renderer;

UniformBuffer::UniformBuffer(GLuint slot, GLsizeiptr size, GLenum usage)
	: slot(slot),
	  size(size),
	  usage(usage)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, id);
}

UniformBuffer::~UniformBuffer()
{
	// ID must not be zero
	if (id)
	{
		glDeleteBuffers(1, &id);
	}
}