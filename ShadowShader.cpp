#include "ShadowShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/ShadowVS.glsl",
	FRAGMENT_PATH = "shaders/ShadowFS.glsl",
	GEOMETRY_PATH = "shaders/ShadowGS.glsl";

ShadowShader::ShadowShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH, GEOMETRY_PATH)
{
	GetUniformLocations();
}

void ShadowShader::GetUniformLocations()
{
}