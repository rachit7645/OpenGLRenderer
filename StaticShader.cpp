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
}

void StaticShader::LoadTransformationMatrix(const glm::mat4 &matrix)
{
	LoadMatrix(uniforms["modelMatrix"], matrix);
}

void StaticShader::LoadProjectionMatrix(const glm::mat4 &matrix)
{
	LoadMatrix(uniforms["projectionMatrix"], matrix);
}

void StaticShader::LoadViewMatrix(const Camera &camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(uniforms["viewMatrix"], viewMatrix);
}

void StaticShader::LoadLight(const Light &light)
{
	LoadVector(uniforms["lightPosition"], light.position);
	LoadVector(uniforms["lightColour"], light.colour);
}

void StaticShader::LoadMaterials(const Material& material)
{
	LoadFloat(uniforms["shineDamper"], material.shineDamper);
	LoadFloat(uniforms["reflectivity"], material.reflectivity);
	LoadBool(uniforms["useFakeLighting"], material.useFakeLighting);
}