#ifndef SHADOW_SHADER_H
#define SHADOW_SHADER_H

#include "Shader.h"

namespace Shader
{
    class ShadowShader : public ShaderProgram
    {
    public:
        // Constructor
        ShadowShader();
        // Get all uniform locations (Unused in this shader)
        void GetUniformLocations() override;
    };
}

#endif
