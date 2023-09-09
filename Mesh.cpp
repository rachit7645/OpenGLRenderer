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
    const AABB& aabb
)
    :
      id(hash),
      vao(std::make_shared<VertexArray>()),
      textures(std::move(textures)),
      aabb(aabb)
{
    // Add data to VAO
    vao->AppendData(vertices, indices, 0, 0);
}