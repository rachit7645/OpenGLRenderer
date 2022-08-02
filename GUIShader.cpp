#include "GUIShader.h"

using namespace Shader;

GUIShader::GUIShader()
	: ShaderProgram(GUI_VERTEX_SHADER_PATH, GUI_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void GUIShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
}

void GUIShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}
