#include "UniformBuffer.h"

using namespace Renderer;

UniformBuffer::UniformBuffer(GLuint slot, GLsizeiptr size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, id);
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &id);
}