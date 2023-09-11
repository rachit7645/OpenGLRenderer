#include "VertexBuffer.h"
#include "Log.h"

using namespace Renderer;

void VertexBuffer::CreateBuffer()
{
    // Generate buffer
    glGenBuffers(1, &id);
}

void VertexBuffer::BufferData(GLenum type, GLintptr offset, const std::vector<Vertex>& data) const
{
    // Get parameters
    usize offsetBytes = offset * sizeof(Vertex);
    usize sizeBytes   = data.size() * sizeof(Vertex);
    // Quick check
    assert(static_cast<GLsizeiptr>(offsetBytes + sizeBytes) <= size);
    // Get pointer
    auto ptr = glMapBufferRange
    (
        type,
        static_cast<GLintptr>(offsetBytes),
        static_cast<GLsizeiptr>(sizeBytes),
        GL_MAP_WRITE_BIT
    );
    // Copy
    std::memcpy(ptr, data.data(), sizeBytes);
    // Unmap pointer
    glUnmapBuffer(type);
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

void VertexBuffer::BufferData(GLenum type, GLintptr offset, const std::vector<GLuint>& data) const
{
    // Get parameters
    usize offsetBytes = offset * sizeof(GLuint);
    usize sizeBytes   = data.size() * sizeof(GLuint);
    // Quick check
    assert(static_cast<GLsizeiptr>(offsetBytes + sizeBytes) <= size);
    // Get pointer
    auto ptr = glMapBufferRange
    (
        type,
        static_cast<GLintptr>(offsetBytes),
        static_cast<GLsizeiptr>(sizeBytes),
        GL_MAP_WRITE_BIT
    );
    // Copy
    std::memcpy(ptr, data.data(), sizeBytes);
    // Unmap pointer
    glUnmapBuffer(type);
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

void VertexBuffer::ReAllocateMemory(GLenum type, GLsizeiptr count, GLsizeiptr elementSize)
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
        count += oldSize / elementSize;
    }

    // Create new buffer
    CreateBuffer();
    // Bind as required type
    Bind(type);
    // Allocate more memory (x2 as much to avoid more allocations)
    AllocateMemory(type, 2 * count * elementSize);

    // Bind new VBO as copy write type
    Bind(GL_COPY_WRITE_BUFFER);
    // Copy data on GPU
    glCopyBufferSubData
    (
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        0,
        0,
        oldSize
    );

    // Unbind both buffers
    Unbind(GL_COPY_READ_BUFFER);
    Unbind(GL_COPY_WRITE_BUFFER);

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