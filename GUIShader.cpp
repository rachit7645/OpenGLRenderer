#include "GUIShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/guiVertexShader.glsl",
	FRAGMENT_PATH = "shaders/guiFragmentShader.glsl";

GUIShader::GUIShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void GUIShader::GetUniformLocations()
{
	m_uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
}

void GUIShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(m_uniforms["modelMatrix"], matrix);
}
