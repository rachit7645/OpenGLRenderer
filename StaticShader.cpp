#include "StaticShader.h"

using namespace Shader;

using Entities::Light;
using Entities::Camera;
using Renderer::Material;

StaticShader::StaticShader() : ShaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void StaticShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
	uniforms["shineDamper"] = GetUniformLocation("shineDamper");
	uniforms["reflectivity"] = GetUniformLocation("reflectivity");
	uniforms["useFakeLighting"] = GetUniformLocation("useFakeLighting");
	uniforms["skyColour"] = GetUniformLocation("skyColour");
}

void StaticShader::LoadTransformationMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}

void StaticShader::LoadMaterials(const Material& material)
{
	LoadUniform(uniforms["shineDamper"], material.shineDamper);
	LoadUniform(uniforms["reflectivity"], material.reflectivity);
	LoadUniform(uniforms["useFakeLighting"], material.useFakeLighting);
}

void StaticShader::LoadSkyColour(const glm::vec4& skyColour)
{
	LoadUniform(uniforms["skyColour"], skyColour);
}