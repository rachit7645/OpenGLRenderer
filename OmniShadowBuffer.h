#ifndef OMNI_SHADOW_BUFFER_H
#define OMNI_SHADOW_BUFFER_H

#include <vector>

#include <GL/glew.h>

#include "GLM.h"
#include "UniformBuffer.h"
#include "Util.h"
#include "GL.h"
#include "LightsBuffer.h"

namespace Renderer
{
    // Constants
    constexpr usize SHADOW_CUBE_MAP_FACES = 6;

    class OmniShadowBuffer : public UniformBuffer
    {
    public:
        // Usings
        using Mat4s = std::vector<glm::mat4>;

        // GLSL representation of shadow buffer
        struct ALIGN_GLSL_STD140 PointShadowGLSL
        {
            // Shadow matrices
            glm::mat4 matrices[SHADOW_CUBE_MAP_FACES] = {};
            // Near and far planes
            glm::vec4 shadowPlanes = {};
            // Light position
            GL::Int lightIndex = {};
        };

        // GLSL representation of shadow buffer
        struct ALIGN_GLSL_STD140 OmniShadowBufferGLSL
        {
            // Shadow maps
            PointShadowGLSL omniShadowMaps[SHADER_MAX_LIGHTS] = {};
            // Current light index
            GL::Int currentIndex = {};
        };

        // Main constructor
        OmniShadowBuffer();

        // Load shadow map data
        void LoadShadowMap(const Mat4s& matrices, glm::vec2 shadowPlanes, usize lightIndex);
        // Load current light index
        void LoadLightIndex(usize lightIndex);
    };
}

#endif
