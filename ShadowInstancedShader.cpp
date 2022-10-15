#include "ShadowInstancedShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/shadowInstancedVertexShader.glsl",
	FRAGMENT_PATH = "shaders/shadowInstancedFragmentShader.glsl",
	GEOMETRY_PATH = "shaders/shadowInstancedGeometryShader.glsl";

ShadowInstancedShader::ShadowInstancedShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH, GEOMETRY_PATH)
{
	GetUniformLocations();
}

void ShadowInstancedShader::GetUniformLocations()
{
}