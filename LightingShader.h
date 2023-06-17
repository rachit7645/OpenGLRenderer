#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "Shader.h"

namespace Shader
{
    class LightingShader : public ShaderProgram
    {
    public:
        // Constructor
        LightingShader();

        // Connect GPU texture units
        void ConnectTextureUnits();

        // Get all uniform locations
        void GetUniformLocations() override;
    };
}

#endif
