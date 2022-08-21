#include "WaterShader.h"

using namespace Shader;

WaterShader::WaterShader()
	: ShaderProgram(WATER_VERTEX_SHADER_PATH, WATER_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void WaterShader::GetUniformLocations()
{
	uniforms["modelMatrix"]  = GetUniformLocation("modelMatrix");
	uniforms["reflectionTx"] = GetUniformLocation("reflectionTx");
	uniforms["refractionTx"] = GetUniformLocation("refractionTx");
	uniforms["moveFactor"]   = GetUniformLocation("moveFactor");
}

void WaterShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}

void WaterShader::LoadMoveFactor(f32 moveFactor)
{
	LoadUniform(uniforms["moveFactor"], moveFactor);
}

void WaterShader::ConnectTextureUnits()
{
	LoadUniform(uniforms["reflectionTx"], 0);
	LoadUniform(uniforms["refractionTx"], 1);
	LoadUniform(uniforms["dudvMap"],      2);
}
