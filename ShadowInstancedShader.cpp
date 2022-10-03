#include "ShadowInstancedShader.h"

using namespace Shader;

using Entities::Light;

ShadowInstancedShader::ShadowInstancedShader()
	: ShaderProgram(SHADOW_INSTANCED_SHADER_VERTEX_PATH, SHADOW_INSTANCED_SHADER_FRAGMENT_PATH)
{
	GetUniformLocations();
}

void ShadowInstancedShader::GetUniformLocations()
{
}