#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "ShaderStorageBuffer.h"
#include "Entity.h"
#include "GLM.h"
#include "Util.h"
#include "GL.h"

namespace Renderer
{
	// Maximum entities allowed in one batch
	constexpr usize NUM_MAX_ENTITIES = 512;

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
            InstanceDataGLSL instances[NUM_MAX_ENTITIES];
        };

        // Usings
        using DataVector = std::vector<InstanceBuffer::InstanceDataGLSL>;
        using EntityVector = std::vector<Entities::Entity*>;

        // Main constructor
        InstanceBuffer();

        // Bind buffer
        void Bind() const;
        // Unbind buffer
        void Unbind() const;

        // Load instance data
        void LoadInstanceData(const EntityVector& entities);
    private:
        // Create instance data
        DataVector GenerateData(const EntityVector& entities);
        // Get size of instance data
        usize GetSize(const EntityVector& entities);
    };
}

#endif
