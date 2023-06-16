#ifndef OMNI_SHADOW_BUFFER_H
#define OMNI_SHADOW_BUFFER_H

#include <vector>

#include <GL/glew.h>

#include "GLM.h"
#include "UniformBuffer.h"
#include "Util.h"
#include "GL.h"

namespace Renderer
{
    // Constants
    constexpr usize SHADOW_CUBE_MAP_FACES = 6;

    class OmniShadowBuffer : public UniformBuffer
    {
    public:
        // Usings
        using Mat4s = std::vector<glm::mat4>;

        // Internal representation of GLSL data
        struct ALIGN_GLSL_STD140 OmniShadowBufferGLSL
        {
            // Shadow matrices
            glm::mat4 matrices[SHADOW_CUBE_MAP_FACES] = {};
            // Near and far planes
            glm::vec4 shadowPlanes = {};
            // Light position
            GL::Int lightIndex = {};
        };

        // Main constructor
        OmniShadowBuffer();

        // Load shadow matrices
        void LoadMatrices(const Mat4s& matrices);
        // Load shadow planes
        void LoadShadowPlanes(const glm::vec2& planes);
        // Load light position
        void LoadLightIndex(usize index);
    };
}

#endif
