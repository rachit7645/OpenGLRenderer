#ifndef OMNI_SHADOW_SHADER_H
#define OMNI_SHADOW_SHADER_H

#include "Shader.h"

namespace Shader
{
    class OmniShadowShader : public ShaderProgram
    {
    public:
        // Constructor
        OmniShadowShader();

        // Get all uniform locations
        void GetUniformLocations() override;
    };
}

#endif
