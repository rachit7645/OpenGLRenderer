#include "SkyboxShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/SkyboxVS.glsl",
    FRAGMENT_PATH = "shaders/SkyboxFS.glsl";

SkyboxShader::SkyboxShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    // Query uniforms
    GetUniformLocations();
}


// Unused
void SkyboxShader::GetUniformLocations()
{
}