#include "FastInstancedShader.h"

using namespace Shader;

FastInstancedShader::FastInstancedShader()
	: ShaderProgram(FAST_INSTANCED_VERTEX_SHADER_PATH, FAST_INSTANCED_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void FastInstancedShader::GetUniformLocations()
{
}