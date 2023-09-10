#include "VertexArray.h"
#include "InstancedRenderer.h"
#include "DrawCall.h"

// Using namespaces
using namespace Renderer;

VertexArray::VertexArray(usize vertexCount, usize indexCount)
{
    // Create VAO
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);

    // Enable attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // Create vertex buffer
    buffers["vertices"] = std::make_shared<VertexBuffer>();
    buffers["vertices"]->CreateBuffer();
    buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
    // Pre allocate required memory
    buffers["vertices"]->AllocateMemory(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexCount * sizeof(Vertex)));

    // Enable position
    buffers["vertices"]->SetVertexAttribute
    (
        0,
        3,
        GL_FLOAT,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );

    // Enable texture coords
    buffers["vertices"]->SetVertexAttribute
    (
        1,
        2,
        GL_FLOAT,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, txCoord))
    );

    // Enable normals
    buffers["vertices"]->SetVertexAttribute
    (
        2,
        3,
        GL_FLOAT,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal))
    );

    // Enable tangents
    buffers["vertices"]->SetVertexAttribute
    (
        3,
        3,
        GL_FLOAT,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, tangent))
    );

    // Unbind
    buffers["vertices"]->Unbind(GL_ARRAY_BUFFER);

    // Create index buffer
    buffers["indices"] = std::make_shared<VertexBuffer>();
    buffers["indices"]->CreateBuffer();
    buffers["indices"]->Bind(GL_ELEMENT_ARRAY_BUFFER);
    // Pre allocate index memory
    buffers["indices"]->AllocateMemory(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexCount * sizeof(GLuint)));

    // Unbind
    glBindVertexArray(0);
}

VertexArray::VertexArray
(
    GLint coordSize,
    const std::vector<f32>& vertices
)
    : vertexCount(static_cast<GLsizei>(vertices.size() / coordSize))
{
    // Create VAO
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    // Enable attributes
    glEnableVertexAttribArray(0);

    // Create vertex buffer
    buffers["vertices"] = std::make_shared<VertexBuffer>();
    buffers["vertices"]->CreateBuffer();
    buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
    buffers["vertices"]->BufferData(GL_ARRAY_BUFFER, vertices);
    buffers["vertices"]->SetVertexAttribute(0, coordSize, GL_FLOAT, 0, nullptr);
    buffers["vertices"]->Unbind(GL_ARRAY_BUFFER);

    // Unbind
    glBindVertexArray(0);
}

void VertexArray::AppendData
(
    const std::vector<Vertex>& vertices,
    const std::vector<u32>& indices,
    usize vertexOffset,
    usize indexOffset
)
{
    // Bind VAO
    glBindVertexArray(id);

    // Allocate more memory if needed
    if (CheckMemory(buffers["vertices"], vertices.size(), vertexOffset, sizeof(Vertex)))
    {
        // Get element count
        auto count = static_cast<GLsizeiptr>(vertexOffset + vertices.size());
        // Allocate more memory
        buffers["vertices"]->ReAllocateMemory(GL_ARRAY_BUFFER, count, sizeof(Vertex));
    }

    // Allocate more memory if needed
    if (CheckMemory(buffers["indices"], indices.size(), indexOffset, sizeof(GLuint)))
    {
        // Get element count
        auto count = static_cast<GLsizeiptr>(indexOffset + indices.size());
        // Allocate more memory
        buffers["indices"]->ReAllocateMemory(GL_ELEMENT_ARRAY_BUFFER, count, sizeof(GLuint));
    }

    // Bind vertex data
    buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
    // Buffer vertex data
    buffers["vertices"]->BufferData(GL_ARRAY_BUFFER, static_cast<GLintptr>(vertexOffset), vertices);

    // Bind index data
    buffers["indices"]->Bind(GL_ELEMENT_ARRAY_BUFFER);
    // Buffer index data
    buffers["indices"]->BufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(indexOffset), indices);

    // Set vertex count
    vertexCount += static_cast<GLsizei>(indices.size());

    // Unbind
    glBindVertexArray(0);
}

bool VertexArray::CheckMemory(const VertexArray::VBO& buffer, usize newCount, usize offset, usize elementSize)
{
    // Check space left and space required
    GLsizeiptr spaceLeft     = buffer->size - static_cast<GLsizeiptr>(offset * elementSize);
    auto       spaceRequired = static_cast<GLsizeiptr>(newCount * elementSize);
    // Return
    return spaceRequired > spaceLeft;
}

VertexArray::~VertexArray()
{
    // ID must not be zero
    if (id)
    {
        // Clear VAO
        glDeleteVertexArrays(1, &id);
    }
}
