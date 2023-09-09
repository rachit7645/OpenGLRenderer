#include "VertexPool.h"

// Using namespaces
using namespace Renderer;

VertexPool::VertexPool()
    : poolVAO(std::make_shared<VertexArray>())
{
}

void VertexPool::AppendData(usize hash, const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
{
    // Load to VAO
    poolVAO->AppendData(vertices, indices, vertexOffset, indexOffset);

    // Calculate vertex and index offsets
    vertexOffset += vertices.size();
    indexOffset  += indices.size();

    // Store to vector
}