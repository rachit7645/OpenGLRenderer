#include "FastInstancedShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/fastInstancedVertexShader.glsl",
	FRAGMENT_PATH = "shaders/fastInstancedFragmentShader.glsl";

FastInstancedShader::FastInstancedShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void FastInstancedShader::GetUniformLocations()
{
}