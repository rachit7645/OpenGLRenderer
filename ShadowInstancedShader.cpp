#include "ShadowInstancedShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/ShadowInstancedVS.glsl",
	FRAGMENT_PATH = "shaders/ShadowInstancedFS.glsl",
	GEOMETRY_PATH = "shaders/ShadowInstancedGS.glsl";

ShadowInstancedShader::ShadowInstancedShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH, GEOMETRY_PATH)
{
	GetUniformLocations();
}

void ShadowInstancedShader::GetUniformLocations()
{
}