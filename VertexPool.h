#ifndef VERTEX_POOL_H
#define VERTEX_POOL_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "VertexArray.h"
#include "Util.h"
#include "GLM.h"
#include "DrawCall.h"

namespace Renderer
{
    // Constants
    constexpr usize PRE_ALLOC_VERTEX_COUNT = 1024;
    constexpr usize PRE_ALLOC_INDEX_COUNT  = 1024;

    class VertexPool
    {
    public:
        // Usings
        using VAO = std::shared_ptr<Renderer::VertexArray>;

        // Mesh information
        struct MeshSpec
        {
            usize vertexOffset;
            usize indexOffset;
            usize vertexCount;
        };

        // Constructor
        VertexPool();

        // Add mesh data
        void AppendData(usize hash, const std::vector<Renderer::Vertex>& vertices, const std::vector<u32>& indices);

        // Main VAO
        VAO poolVAO = nullptr;
        // Mesh map
        std::unordered_map<usize, MeshSpec> specs = {};

    private:
        // Vertex data offset
        usize m_vertexOffset = {};
        // Index data offset
        usize m_indexOffset = {};
    };
}

#endif
