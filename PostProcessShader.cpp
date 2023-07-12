#include "PostProcessShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/PostProcessVS.glsl",
    FRAGMENT_PATH = "shaders/PostProcessFS.glsl";

PostProcessShader::PostProcessShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    // Query uniform locations
    GetUniformLocations();
}

void PostProcessShader::GetUniformLocations()
{
    // Uniforms
    m_uniforms["lightingBuffer"] = GetUniformLocation("lightingBuffer");
    m_uniforms["bloomBuffer"]    = GetUniformLocation("bloomBuffer");
    m_uniforms["bloomStrength"]  = GetUniformLocation("bloomStrength");
}

void PostProcessShader::ConnectTextureUnits()
{
    // Connect units
    LoadUniform(m_uniforms["lightingBuffer"], 0);
    LoadUniform(m_uniforms["bloomBuffer"],    1);
}

void PostProcessShader::LoadBloomStrength(f32 strength)
{
    // Load
    LoadUniform(m_uniforms["bloomStrength"], strength);
}