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
	uniforms["modelMatrix"]		  = GetUniformLocation("modelMatrix");
	uniforms["shineDamper"]		  = GetUniformLocation("shineDamper");
	uniforms["reflectivity"]	  = GetUniformLocation("reflectivity");
	uniforms["skyColour"]		  = GetUniformLocation("skyColour");

	uniforms["backgroundTexture"] = GetUniformLocation("backgroundTexture");
	uniforms["rTexture"] 		  = GetUniformLocation("rTexture");
	uniforms["gTexture"] 		  = GetUniformLocation("gTexture");
	uniforms["bTexture"] 		  = GetUniformLocation("bTexture");
	uniforms["blendMap"] 		  = GetUniformLocation("blendMap");
}

void TerrainShader::LoadTransformationMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}

void TerrainShader::LoadMaterials(const Renderer::Material& material)
{
	LoadUniform(uniforms["shineDamper"],  material.shineDamper);
	LoadUniform(uniforms["reflectivity"], material.reflectivity);
}

void TerrainShader::LoadSkyColour(const glm::vec4& skyColour)
{
	LoadUniform(uniforms["skyColour"], skyColour);
}

void TerrainShader::ConnectTextureUnits()
{
	LoadUniform(uniforms["backgroundTexture"], 0);
	LoadUniform(uniforms["rTexture"], 		   1);
	LoadUniform(uniforms["bTexture"], 		   2);
	LoadUniform(uniforms["gTexture"], 		   3);
	LoadUniform(uniforms["blendMap"], 		   4);
}