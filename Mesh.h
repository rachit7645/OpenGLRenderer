#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>

#include "VertexArray.h"
#include "MeshTextures.h"
#include "Vertex.h"
#include "AABB.h"

namespace Renderer
{
    class Mesh
    {
    public:
        // Usings
        using VAO = std::shared_ptr<VertexArray>;

        // Main constructor
        Mesh
        (
            const std::vector<Vertex>& vertices,
            const std::vector<u32>& indices,
            const Renderer::MeshTextures& textures,
            const Maths::AABB& aabb
        );

        // Vertex Attribute Object
        VAO vao;
        // Textures
        MeshTextures textures;
        // Axis Aligned Bounding Box
        Maths::AABB aabb;
    };
}

#endif