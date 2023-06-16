#include "OmniShadowShader.h"

// Using namespaces
using namespace Shader;

// Shader paths
constexpr auto VERTEX_PATH = "shaders/OmniShadowVS.glsl",
    FRAGMENT_PATH = "shaders/OmniShadowFS.glsl",
    GEOMETRY_PATH = "shaders/OmniShadowGS.glsl";

OmniShadowShader::OmniShadowShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH, GEOMETRY_PATH)
{
    // Get all uniform locations
    GetUniformLocations();
}

void OmniShadowShader::GetUniformLocations()
{
    // Get all uniform locations
}