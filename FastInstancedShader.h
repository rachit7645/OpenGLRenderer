#ifndef FAST_INSTANCED_SHADER_H
#define FAST_INSTANCED_SHADER_H

#include "Shader.h"

namespace Shader
{
    class FastInstancedShader : public ShaderProgram
    {
    public:
        // Constructor
        FastInstancedShader();
        // Connects GL texture units
        void ConnectTextureUnits();
        // Get all uniform locations
        void GetUniformLocations() override;
    };
}

#endif
