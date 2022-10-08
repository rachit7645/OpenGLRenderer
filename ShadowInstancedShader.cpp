#include "ShadowInstancedShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/shadowInstancedVertexShader.glsl",
	FRAGMENT_PATH = "shaders/shadowInstancedFragmentShader.glsl";

ShadowInstancedShader::ShadowInstancedShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void ShadowInstancedShader::GetUniformLocations()
{
}