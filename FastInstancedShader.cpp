#include "FastInstancedShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/FastInstancedVS.glsl",
    FRAGMENT_PATH = "shaders/FastInstancedFS.glsl";

FastInstancedShader::FastInstancedShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    // Query uniform locations
    GetUniformLocations();
}

void FastInstancedShader::GetUniformLocations()
{
    // Uniforms
    m_uniforms["albedoMap"]     = GetUniformLocation("albedoMap");
    m_uniforms["aoRghMtlMap"]   = GetUniformLocation("aoRghMtlMap");
    m_uniforms["irradianceMap"] = GetUniformLocation("irradianceMap");
    m_uniforms["prefilterMap"]  = GetUniformLocation("prefilterMap");
    m_uniforms["brdfLUT"]       = GetUniformLocation("brdfLUT");
}

void FastInstancedShader::ConnectTextureUnits()
{
    // Connect units
    LoadUniform(m_uniforms["albedoMap"],     0);
    LoadUniform(m_uniforms["aoRghMtlMap"],   1);
    LoadUniform(m_uniforms["irradianceMap"], 2);
    LoadUniform(m_uniforms["prefilterMap"],  3);
    LoadUniform(m_uniforms["brdfLUT"],       4);
}