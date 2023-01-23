#include "SSAOBlurShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/SSAOBlurVS.glsl",
	FRAGMENT_PATH = "shaders/SSAOBlurFS.glsl";

SSAOBlurShader::SSAOBlurShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	// Query uniform locations
	GetUniformLocations();
}

void SSAOBlurShader::GetUniformLocations()
{
	// Uniforms
	m_uniforms["ssaoInput"] = GetUniformLocation("ssaoInput");
}

void SSAOBlurShader::ConnectTextureUnits()
{
	// Load source texture to unit 0
	LoadUniform(m_uniforms["ssaoInput"], 0);
}