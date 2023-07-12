#ifndef SHADER_STORAGE_BUFFER_H
#define SHADER_STORAGE_BUFFER_H

#include <GL/glew.h>

namespace Renderer
{
    class ShaderStorageBuffer
    {
    public:
        // Default constructor
        ShaderStorageBuffer() = default;
        // Main constructor
        ShaderStorageBuffer(GLuint slot, GLsizeiptr size, GLenum usage);
        // Destructor
        ~ShaderStorageBuffer();
        // Buffer ID
        GLuint id = 0;
        // Buffer slot
        GLuint slot = 0;
        // Buffer max size
        GLsizeiptr size = 0;
        // Buffer usage hint
        GLenum usage = 0;
    };
}

#endif
