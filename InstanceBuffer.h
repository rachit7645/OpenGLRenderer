#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "ShaderStorageBuffer.h"
#include "Entity.h"
#include "GLM.h"
#include "Util.h"
#include "GL.h"
#include "Mesh.h"
#include "DrawCall.h"

namespace Renderer
{
    // Maximum instanced per batch
    constexpr usize NUM_MAX_ENTITIES = 512 * MAX_DRAW_CALLS;

    // Per model instance data
    class InstanceBuffer : ShaderStorageBuffer
    {
    public:
        // GLSL representation of per-instance data
        struct ALIGN_GLSL_STD140 InstanceDataGLSL
        {
            // Model matrix
            glm::mat4 modelMatrix;
            // Normal matrix
            glm::mat4 normalMatrix;
        };

        // GLSL representation of instance buffer
        struct ALIGN_GLSL_STD140 InstanceBufferGLSL
        {
            // Array of instance data
            InstanceDataGLSL instances[NUM_MAX_ENTITIES] = {};
        };

        // Usings
        using MdPtr        = std::shared_ptr<Model>;
        using DataVector   = std::vector<InstanceBuffer::InstanceDataGLSL>;
        using EntityVector = std::vector<Entities::Entity*>;
        using Batch        = std::unordered_map<MdPtr, EntityVector>;

        // Main constructor
        InstanceBuffer();

        // Bind buffer
        void Bind() const;
        // Unbind buffer
        static void Unbind();

        // Load instance data
        void LoadInstanceData(const Batch& batch);
    private:
        // Create instance data
        static DataVector GenerateData(const Batch& batch);
        // Get count
        static usize GetCount(const Batch& batch);
        // Get size
        static usize GetSize(const Batch& batch);
    };
}

#endif
