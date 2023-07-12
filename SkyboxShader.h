#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
    class SkyboxShader : public ShaderProgram
    {
    public:
        // Constructor
        SkyboxShader();
        // Get all uniform locations (unused)
        void GetUniformLocations() override;
    };
}

#endif