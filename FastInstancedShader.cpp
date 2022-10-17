#include "FastInstancedShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/FastInstancedVS.glsl",
	FRAGMENT_PATH = "shaders/FastInstancedFS.glsl";

FastInstancedShader::FastInstancedShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void FastInstancedShader::GetUniformLocations()
{
}