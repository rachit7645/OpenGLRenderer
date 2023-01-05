#include "ShaderStorageBuffer.h"

// Using namespaces
using namespace Renderer;

ShaderStorageBuffer::ShaderStorageBuffer(GLuint slot, GLsizeiptr size, GLenum usage)
	: slot(slot),
	  size(size),
	  usage(usage)
{
	// Generate buffer
	glGenBuffers(1, &id);
	// Bind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	// Load 0s into the buffer
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, usage);
	// Unbind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	// Set buffer slot
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, id);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	// ID must not be equal to zero
	if (id != 0)
	{
		// Clear buffer
		glDeleteBuffers(1, &id);
	}
}