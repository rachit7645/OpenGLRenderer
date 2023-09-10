#include "VertexBuffer.h"

using namespace Renderer;

void VertexBuffer::CreateBuffer()
{
    // Generate buffer
    glGenBuffers(1, &id);
}

void VertexBuffer::BufferData(GLenum type, GLintptr offset, const std::vector<Vertex>& data)
{
    // Buffer packed data
    glBufferSubData
    (
        type,
        static_cast<GLintptr>(offset * sizeof(Vertex)),
        static_cast<GLsizeiptr>(data.size() * sizeof(Vertex)),
        reinterpret_cast<const void*>(data.data())
    );
}

void VertexBuffer::BufferData(GLenum type, const std::vector<GLfloat>& data)
{
    // Buffer f32 data
    glBufferData
    (
        type,
        static_cast<GLsizeiptr>(data.size() * sizeof(GLfloat)),
        data.data(),
        GL_STATIC_DRAW
    );
}

void VertexBuffer::BufferData(GLenum type, GLintptr offset, const std::vector<GLuint>& data)
{
    // Buffer u32 data
    glBufferSubData
    (
        type,
        static_cast<GLintptr>(offset * sizeof(GLuint)),
        static_cast<GLsizeiptr>(data.size() * sizeof(GLuint)),
        reinterpret_cast<const void*>(data.data())
    );
}

void VertexBuffer::AllocateMemory(GLenum type, GLsizeiptr bufferSize)
{
    // Set size
    size = bufferSize;
    // Allocate memory
    glBufferData
    (
        type,
        bufferSize,
        nullptr,
        GL_STATIC_DRAW
    );
}

void VertexBuffer::ReAllocateMemory(GLsizeiptr count, GLsizeiptr elementSize)
{
    // Copy old data
    GLuint     oldBuffer = id;
    GLsizeiptr oldSize   = size;
    // Bind old buffer as read point
    Bind(GL_COPY_READ_BUFFER);

    // Make sure that there is enough memory for copying
    if (count * elementSize < oldSize)
    {
        // New element count
        count = oldSize / elementSize;
    }

    // Create new buffer
    CreateBuffer();
    Bind(GL_COPY_WRITE_BUFFER);
    // Allocate more memory (x2 as much to avoid more allocations)
    AllocateMemory(GL_COPY_WRITE_BUFFER, 2 * count * elementSize);

    // Copy data on GPU
    glCopyBufferSubData
    (
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        0,
        0,
        oldSize
    );

    // Delete old buffer
    glDeleteBuffers(1, &oldBuffer);
}

void VertexBuffer::SetVertexAttribute
(
    GLuint index,
    GLint nComponents,
    GLenum type,
    GLsizei stride,
    const void* pointer
)
{
    // Set vertex attribute
    glVertexAttribPointer
    (
        index,
        nComponents,
        type,
        GL_FALSE,
        stride,
        pointer
    );
}

void VertexBuffer::Bind(GLenum type) const
{
    // Bind
    glBindBuffer(type, id);
}

void VertexBuffer::Unbind(GLenum type)
{
    // Unbind
    glBindBuffer(type, 0);
}

VertexBuffer::~VertexBuffer()
{
    // ID must not be zero
    if (id)
    {
        // Clear buffer
        glDeleteBuffers(1, &id);
    }
}