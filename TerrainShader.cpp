#include "TerrainShader.h"

using namespace Shader;

using Entities::Light;
using Entities::Camera;
using Terrains::Terrain;

TerrainShader::TerrainShader() : ShaderProgram(TERRAIN_VERTEX_SHADER_PATH, TERRAIN_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void TerrainShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
	uniforms["projectionMatrix"] = GetUniformLocation("projectionMatrix");
	uniforms["viewMatrix"] = GetUniformLocation("viewMatrix");
	uniforms["lightPosition"] = GetUniformLocation("lightPosition");
	uniforms["lightColour"] = GetUniformLocation("lightColour");
	uniforms["shineDamper"] = GetUniformLocation("shineDamper");
	uniforms["reflectivity"] = GetUniformLocation("reflectivity");
	uniforms["skyColour"] = GetUniformLocation("skyColour");

	uniforms["backgroundTexture"] = GetUniformLocation("backgroundTexture");
	uniforms["rTexture"] = GetUniformLocation("rTexture");
	uniforms["gTexture"] = GetUniformLocation("gTexture");
	uniforms["bTexture"] = GetUniformLocation("bTexture");
	uniforms["blendMap"] = GetUniformLocation("blendMap");
}

void TerrainShader::LoadTransformationMatrix(const glm::mat4 &matrix)
{
	LoadMatrix(uniforms["modelMatrix"], matrix);
}

void TerrainShader::LoadProjectionMatrix(const glm::mat4 &matrix)
{
	LoadMatrix(uniforms["projectionMatrix"], matrix);
}

void TerrainShader::LoadViewMatrix(const Camera &camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(uniforms["viewMatrix"], viewMatrix);
}

void TerrainShader::LoadLight(const Light& light)
{
	LoadVector(uniforms["lightPosition"], light.position);
	LoadVector(uniforms["lightColour"], light.colour);
}

void TerrainShader::LoadMaterials(const Renderer::Material& material)
{
	LoadFloat(uniforms["shineDamper"], material.shineDamper);
	LoadFloat(uniforms["reflectivity"], material.reflectivity);
}

void TerrainShader::LoadSkyColour(const glm::vec4& skyColour)
{
	LoadVector(uniforms["skyColour"], skyColour);
}

void TerrainShader::ConnectTextureUnits()
{
	LoadInt(uniforms["backgroundTexture"], 0);
	LoadInt(uniforms["rTexture"], 1);
	LoadInt(uniforms["bTexture"], 2);
	LoadInt(uniforms["gTexture"], 3);
	LoadInt(uniforms["blendMap"], 4);
}