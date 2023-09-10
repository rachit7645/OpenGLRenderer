#include "VertexPool.h"

// Using namespaces
using namespace Renderer;

VertexPool::VertexPool()
    : poolVAO(std::make_shared<VertexArray>(PRE_ALLOC_VERTEX_COUNT, PRE_ALLOC_INDEX_COUNT))
{
}

void VertexPool::AppendData(usize hash, const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
{
    // Load to VAO
    poolVAO->AppendData(vertices, indices, m_vertexOffset, m_indexOffset);

    // Store to map
    specs[hash] = {m_vertexOffset, m_indexOffset, indices.size()};

    // Calculate new vertex and index offsets
    m_vertexOffset += vertices.size();
    m_indexOffset  += indices.size();
}