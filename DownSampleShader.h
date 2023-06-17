#ifndef DOWN_SAMPLE_SHADER_H
#define DOWN_SAMPLE_SHADER_H

#include "Shader.h"

namespace Shader
{
    class DownSampleShader : public ShaderProgram
    {
    public:
        // Constructor
        DownSampleShader();
        // Set first sample flag
        void SetIsFirstSample(bool firstSample);
        // Connect texture units
        void ConnectTextureUnits();
        // Get all uniform locations
        void GetUniformLocations() override;
    };
}

#endif
