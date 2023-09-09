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

namespace Renderer
{
    // Maximum entities allowed in one batch
    constexpr usize NUM_MAX_ENTITIES = 512;

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
            GL::Int instanceCount = {};
            // Array of instance data
            InstanceDataGLSL instances[NUM_MAX_ENTITIES] = {};
        };

        // Usings
        using DataVector = std::vector<InstanceBuffer::InstanceDataGLSL>;
        using EntityVector = std::vector<Entities::Entity*>;

        // Main constructor
        InstanceBuffer();

        // Bind buffer
        void Bind() const;
        // Unbind buffer
        static void Unbind();

        // Load instance data
        void LoadInstanceData(const EntityVector& entities);
    private:
        // Create instance data
        static DataVector GenerateData(const EntityVector& entities);
        // Get instance count
        static usize GetCount(const EntityVector& entities);
        // Get size of instance data
        static usize GetSize(const EntityVector& entities);
    };
}

#endif
