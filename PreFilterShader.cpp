#include "PreFilterShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/PreFilterVS.glsl",
	FRAGMENT_PATH = "shaders/PreFilterFS.glsl";

PreFilterShader::PreFilterShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void PreFilterShader::LoadProjection(const glm::mat4& projection)
{
	LoadUniform(m_uniforms["projection"], projection);
}

void PreFilterShader::LoadView(const glm::mat4& view)
{
	LoadUniform(m_uniforms["view"], view);
}

void PreFilterShader::LoadRoughness(f32 roughness)
{
	LoadUniform(m_uniforms["roughness"], roughness);
}

void PreFilterShader::LoadResolution(const glm::vec2& resolution)
{
	LoadUniform(m_uniforms["resolution"], resolution);
}

void PreFilterShader::GetUniformLocations()
{
	m_uniforms["envMap"]     = GetUniformLocation("envMap");
	m_uniforms["projection"] = GetUniformLocation("projection");
	m_uniforms["view"]       = GetUniformLocation("view");
	m_uniforms["roughness"]  = GetUniformLocation("roughness");
	m_uniforms["resolution"] = GetUniformLocation("resolution");
}

void PreFilterShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["envMap"], 0);
}