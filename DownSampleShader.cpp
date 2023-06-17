#include "DownSampleShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/DownSampleVS.glsl",
    FRAGMENT_PATH = "shaders/DownSampleFS.glsl";

DownSampleShader::DownSampleShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    // Query uniform locations
    GetUniformLocations();
}

void DownSampleShader::GetUniformLocations()
{
    // Uniforms
    m_uniforms["srcTexture"]    = GetUniformLocation("srcTexture");
    m_uniforms["isFirstSample"] = GetUniformLocation("isFirstSample");
}

void DownSampleShader::ConnectTextureUnits()
{
    // Connect texture
    LoadUniform(m_uniforms["srcTexture"], 0);
}

void DownSampleShader::SetIsFirstSample(bool firstSample)
{
    // Load flag
    LoadUniform(m_uniforms["isFirstSample"], firstSample);
}