#ifndef SPOT_SHADOW_SHADER_H
#define SPOT_SHADOW_SHADER_H

#include "Shader.h"

namespace Shader
{
    class SpotShadowShader : public ShaderProgram
    {
    public:
        // Constructor
        SpotShadowShader();

        // Get all uniform locations
        void GetUniformLocations() override;
    };
}

#endif
