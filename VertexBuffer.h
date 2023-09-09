#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "Util.h"
#include "Vertex.h"

namespace Renderer
{
    class VertexBuffer
    {
    public:
        // Default constructor
        VertexBuffer() = default;
        // Destructor
        ~VertexBuffer();

        // Bind vertex buffer
        void Bind(GLenum type) const;
        // Unbind vertex buffer
        void Unbind(GLenum type) const;

        // Buffer ID
        GLuint id = 0;
        // Buffer size
        GLsizeiptr size = 0;
        // Memory allocation flag
        bool isMemoryAllocated = false;
    protected:
        // Creates a buffer
        void CreateBuffer();

        // Buffer packed vertex data
        void BufferData(GLenum type, GLintptr offset, const std::vector<Vertex>& data);
        // Buffer floating point vertex data
        void BufferData(GLenum type, const std::vector<GLfloat>& data);
        // Buffer unsigned vertex data
        void BufferData(GLenum type, GLintptr offset, const std::vector<GLuint>& data);

        // Allocate space for data
        void AllocateMemory(GLenum type, GLsizeiptr bufferSize);

        // Set vertex attribute
        void SetVertexAttribute
        (
            GLuint index,
            GLint nComponents,
            GLenum type,
            GLsizei stride,
            const void* pointer
        );
    public:
        // Classes that can access pipeline mode
        friend class VertexArray;
        friend class VertexPool;
    };
}

#endif