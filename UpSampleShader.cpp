#include "UpSampleShader.h"

// Using namespaces
using namespace Shader;

// Constants
constexpr auto VERTEX_PATH = "shaders/UpSampleVS.glsl",
	FRAGMENT_PATH = "shaders/UpSampleFS.glsl";

UpSampleShader::UpSampleShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	// Query uniform locations
	GetUniformLocations();
}

void UpSampleShader::GetUniformLocations()
{
	// Uniforms
	m_uniforms["srcTexture"]   = GetUniformLocation("srcTexture");
	m_uniforms["filterRadius"] = GetUniformLocation("filterRadius");
}

void UpSampleShader::ConnectTextureUnits()
{
	// Load source texture to unit 0
	LoadUniform(m_uniforms["srcTexture"], 0);
}

void UpSampleShader::LoadFilterRadius(f32 radius)
{
	// Load filter radius
	LoadUniform(m_uniforms["filterRadius"], radius);
}