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
	m_uniforms["lightingBuffer"] = GetUniformLocation("lightingBuffer");
}

void PostProcessShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["lightingBuffer"], 0);
}