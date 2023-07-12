#ifndef SPOT_SHADOW_BUFFER_H
#define SPOT_SHADOW_BUFFER_H

#include <GL/glew.h>

#include "GLM.h"
#include "UniformBuffer.h"
#include "Util.h"
#include "GL.h"
#include "LightsBuffer.h"

namespace Renderer
{
    class SpotShadowBuffer : public UniformBuffer
    {
    public:
        struct ALIGN_GLSL_STD140 SpotShadowGLSL
        {
            // Shadow matrix
            glm::mat4 shadowMatrix = {};
            // Shadow light index
            GL::Int lightIndex = {};
        };

        struct ALIGN_GLSL_STD140 SpotShadowBufferGLSL
        {
            // Shadow maps
            SpotShadowGLSL spotShadows[SHADER_MAX_LIGHTS] = {};
            // Current light index
            GL::Int currentIndex;
        };

        // Main constructor
        SpotShadowBuffer();

        // Load shadow map data
        void LoadShadowMap(usize lightIndex, const glm::mat4& shadowMatrix);
        // Load current light index
        void LoadCurrentLightIndex(usize lightIndex);
    };
}

#endif
