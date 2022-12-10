#include "ConverterShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/ConverterVS.glsl",
	FRAGMENT_PATH = "shaders/ConverterFS.glsl";

ConverterShader::ConverterShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void ConverterShader::LoadProjection(const glm::mat4& projection)
{
	LoadUniform(m_uniforms["projection"], projection);
}

void ConverterShader::LoadView(const glm::mat4& view)
{
	LoadUniform(m_uniforms["view"], view);
}

void ConverterShader::GetUniformLocations()
{
	m_uniforms["hdrMap"]     = GetUniformLocation("hdrMap");
	m_uniforms["projection"] = GetUniformLocation("projection");
	m_uniforms["view"]       = GetUniformLocation("view");
}

void ConverterShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["hdrMap"], 0);
}