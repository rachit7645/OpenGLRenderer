#include "SpotShadowShader.h"

// Using namespaces
using namespace Shader;

// Shader paths
constexpr auto VERTEX_PATH = "shaders/SpotShadowVS.glsl",
    FRAGMENT_PATH = "shaders/SpotShadowFS.glsl";

SpotShadowShader::SpotShadowShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    // Get all uniform locations
    GetUniformLocations();
}

void SpotShadowShader::GetUniformLocations()
{
    // Get all uniform locations
}