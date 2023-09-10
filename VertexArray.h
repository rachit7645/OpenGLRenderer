#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "Util.h"
#include "Vertex.h"

namespace Renderer
{
    class VertexArray
    {
    public:
        // Usings
        using VBO = std::shared_ptr<VertexBuffer>;

        // Init default VAO
        explicit VertexArray(usize vertexCount, usize indexCount);
        // Init VAO for glDrawArrays
        VertexArray(GLint coordSize, const std::vector<f32>& vertices);

        // Add vertex data
        void AppendData
        (
            const std::vector<Renderer::Vertex>& vertices,
            const std::vector<u32>& indices,
            usize vertexOffset,
            usize indexOffset
        );

        // Destructor
        ~VertexArray();

        // Data
        GLuint  id          = 0;
        GLsizei vertexCount = 0;

        // Vertex buffers
        std::unordered_map<std::string_view, VBO> buffers;
    private:
        // Check whether enough memory is allocated or not
        static bool CheckMemory(const VBO& buffer, usize newCount, usize offset, usize elementSize);
    };
}

#endif