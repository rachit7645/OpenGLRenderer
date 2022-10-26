#include "SSAOShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/AmbientOcclusionVS.glsl",
	FRAGMENT_PATH = "shaders/AmbientOcclusionFS.glsl";

SSAOShader::SSAOShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void SSAOShader::GetUniformLocations()
{
	m_uniforms["gPosition"]   = GetUniformLocation("gPosition");
	m_uniforms["gNormal"]     = GetUniformLocation("gNormal");
	m_uniforms["noiseBuffer"] = GetUniformLocation("noiseBuffer");
}

void SSAOShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["gPosition"],   0);
	LoadUniform(m_uniforms["gNormal"],     1);
	LoadUniform(m_uniforms["noiseBuffer"], 2);
}