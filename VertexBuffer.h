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
        static void Unbind(GLenum type) ;

        // Buffer ID
        GLuint id = 0;
        // Buffer size
        GLsizeiptr size = 0;
    protected:
        // Creates a buffer
        void CreateBuffer();

        // Buffer packed vertex data
        static void BufferData(GLenum type, GLintptr offset, const std::vector<Vertex>& data);
        // Buffer floating point vertex data
        static void BufferData(GLenum type, const std::vector<GLfloat>& data);
        // Buffer unsigned vertex data
        static void BufferData(GLenum type, GLintptr offset, const std::vector<GLuint>& data);

        // Allocate space for data
        void AllocateMemory(GLenum type, GLsizeiptr bufferSize);
        // Reallocate more memory
        void ReAllocateMemory(GLenum type, GLsizeiptr count, GLsizeiptr elementSize);

        // Set vertex attribute
        static void SetVertexAttribute
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
        friend class InstancedRenderer;
    };
}

#endif