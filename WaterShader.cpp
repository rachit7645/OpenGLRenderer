#include "WaterShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/waterVertexShader.glsl",
	FRAGMENT_PATH = "shaders/waterFragmentShader.glsl";

WaterShader::WaterShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void WaterShader::GetUniformLocations()
{
	m_uniforms["modelMatrix"]  = GetUniformLocation("modelMatrix");
	m_uniforms["reflectionTx"] = GetUniformLocation("reflectionTx");
	m_uniforms["refractionTx"] = GetUniformLocation("refractionTx");
	m_uniforms["moveFactor"]   = GetUniformLocation("moveFactor");
	m_uniforms["normalMap"]    = GetUniformLocation("normalMap");
	m_uniforms["refractDepth"] = GetUniformLocation("refractDepth");
}

void WaterShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(m_uniforms["modelMatrix"], matrix);
}

void WaterShader::LoadMoveFactor(f32 moveFactor)
{
	LoadUniform(m_uniforms["moveFactor"], moveFactor);
}

void WaterShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["reflectionTx"], 0);
	LoadUniform(m_uniforms["refractionTx"], 1);
	LoadUniform(m_uniforms["dudvMap"],      2);
	LoadUniform(m_uniforms["normalMap"],    3);
}
