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
	uniforms["projectionMatrix"] = GetUniformLocation("projectionMatrix");
	uniforms["viewMatrix"] = GetUniformLocation("viewMatrix");
	uniforms["lightPosition"] = GetUniformLocation("lightPosition");
	uniforms["lightColour"] = GetUniformLocation("lightColour");
	uniforms["shineDamper"] = GetUniformLocation("shineDamper");
	uniforms["reflectivity"] = GetUniformLocation("reflectivity");
	uniforms["useFakeLighting"] = GetUniformLocation("useFakeLighting");
	uniforms["skyColour"] = GetUniformLocation("skyColour");
}

void StaticShader::LoadTransformationMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}

void StaticShader::LoadProjectionMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["projectionMatrix"], matrix);
}

void StaticShader::LoadViewMatrix(const Camera& camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadUniform(uniforms["viewMatrix"], viewMatrix);
}

void StaticShader::LoadLight(const Light& light)
{
	LoadUniform(uniforms["lightPosition"], light.position);
	LoadUniform(uniforms["lightColour"], light.colour);
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