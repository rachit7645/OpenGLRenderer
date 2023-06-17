#include "ConvolutionShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/ConvolutionVS.glsl",
    FRAGMENT_PATH = "shaders/ConvolutionFS.glsl";

ConvolutionShader::ConvolutionShader()
    : ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
    GetUniformLocations();
}

void ConvolutionShader::LoadProjection(const glm::mat4& projection)
{
    LoadUniform(m_uniforms["projection"], projection);
}

void ConvolutionShader::LoadView(const glm::mat4& view)
{
    LoadUniform(m_uniforms["view"], view);
}

void ConvolutionShader::GetUniformLocations()
{
    m_uniforms["envMap"]     = GetUniformLocation("envMap");
    m_uniforms["projection"] = GetUniformLocation("projection");
    m_uniforms["view"]       = GetUniformLocation("view");
}

void ConvolutionShader::ConnectTextureUnits()
{
    LoadUniform(m_uniforms["envMap"], 0);
}