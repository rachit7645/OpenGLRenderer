#include "UniformBuffer.h"

using namespace Renderer;

UniformBuffer::UniformBuffer(GLuint slot, GLsizeiptr size, GLenum usage)
    : slot(slot),
      size(size),
      usage(usage)
{
    // Generate buffer
    glGenBuffers(1, &id);
    // Bind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    // Load 0s into the buffer
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, usage);
    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Set buffer slot
    glBindBufferBase(GL_UNIFORM_BUFFER, slot, id);
}

UniformBuffer::~UniformBuffer()
{
    // ID must not be zero
    if (id)
    {
        // Clear buffer
        glDeleteBuffers(1, &id);
    }
}