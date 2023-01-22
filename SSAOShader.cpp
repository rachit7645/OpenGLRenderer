#include "SSAOShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/SSAO_VS.glsl",
	FRAGMENT_PATH = "shaders/SSAO_FS.glsl";

SSAOShader::SSAOShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	// Query uniform locations
	GetUniformLocations();
}

void SSAOShader::GetUniformLocations()
{
	// Uniforms
	m_uniforms["gNormal"]  = GetUniformLocation("gNormal");
	m_uniforms["gDepth"]   = GetUniformLocation("gDepth");
	m_uniforms["texNoise"] = GetUniformLocation("texNoise");
}

void SSAOShader::ConnectTextureUnits()
{
	// Load source texture to unit 0
	LoadUniform(m_uniforms["gNormal"],  0);
	LoadUniform(m_uniforms["gDepth"],   1);
	LoadUniform(m_uniforms["texNoise"], 2);
}