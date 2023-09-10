#include "Mesh.h"

#include <utility>

// Using namespaces
using namespace Renderer;

// Usings
using Maths::AABB;

Mesh::Mesh
(
    usize hash,
    const std::vector<Vertex>& vertices,
    const std::vector<u32>& indices,
    MeshTextures textures,
    const AABB& aabb,
    VertexPool& pool
)
    :
      id(hash),
      textures(std::move(textures)),
      aabb(aabb)
{
    // Add data to VAO
    pool.AppendData(id, vertices, indices);
}