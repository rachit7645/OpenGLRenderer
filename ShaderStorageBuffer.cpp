#include "ShaderStorageBuffer.h"

using namespace Renderer;

ShaderStorageBuffer::ShaderStorageBuffer(GLuint slot, GLsizeiptr size, GLenum usage)
	: slot(slot),
	  size(size),
	  usage(usage)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, usage);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, id);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &id);
}