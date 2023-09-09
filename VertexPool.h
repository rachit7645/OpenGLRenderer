#ifndef VERTEX_POOL_H
#define VERTEX_POOL_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "VertexArray.h"
#include "Util.h"
#include "GLM.h"

namespace Renderer
{
    class VertexPool
    {
    public:
        // Usings
        using VAO = std::shared_ptr<Renderer::VertexArray>;

        // Constructor
        VertexPool();

        // Add mesh data
        void AppendData(usize hash, const std::vector<Renderer::Vertex>& vertices, const std::vector<u32>& indices);

        // Main VAO
        VAO poolVAO = nullptr;
        // Mesh map
        std::unordered_map<usize, glm::ivec2> offsets = {};
        // Vertex data offset
        usize vertexOffset = {};
        // Index data offset
        usize indexOffset = {};
    };
}

#endif
