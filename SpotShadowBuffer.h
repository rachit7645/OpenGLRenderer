#ifndef SPOT_SHADOW_BUFFER_H
#define SPOT_SHADOW_BUFFER_H

#include <GL/glew.h>

#include "GLM.h"
#include "UniformBuffer.h"
#include "Util.h"
#include "GL.h"

namespace Renderer
{
    class SpotShadowBuffer : public UniformBuffer
    {
    public:
        struct ALIGN_GLSL_STD140 SpotShadowBufferGLSL
        {
            // Shadow matrix
            glm::mat4 shadowMatrix = {};
            // Shadow light index
            GL::Int lightIndex = {};
        };

        // Main constructor
        SpotShadowBuffer();

        // Load shadow map data
        void LoadShadowMap(usize lightIndex, const glm::mat4& shadowMatrix);
    };
}

#endif
