#include "TerrainRenderer.h"

#include <gl/glew.h>

#include "GLM.h"
#include "Util.h"
#include "Maths.h"

using namespace Renderer;

using Shader::TerrainShader;
using Terrains::TerrainTextures;
using Terrains::Terrain;

TerrainRenderer::TerrainRenderer(TerrainShader& shaderRef) : shader{ shaderRef }
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void TerrainRenderer::Render(const std::vector<Terrain>& terrains)
{
	for (const auto& terrain : terrains)
	{
		PrepareTerrain(terrain);
		LoadModelMatrix(terrain);
		glDrawElements
		(
			GL_TRIANGLES,
			terrain.vao->vertexCount,
			GL_UNSIGNED_INT,
			nullptr
		);
		UnbindTerrain();
	}
}

void TerrainRenderer::PrepareTerrain(const Terrain& terrain)
{
	glBindVertexArray(terrain.vao->id);
	BindTextures(terrain);
	shader.LoadMaterials(terrain.material);
}

void TerrainRenderer::BindTextures(const Terrain& terrain)
{
	const TerrainTextures& textures = terrain.textures;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures.background->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures.rTexture->id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures.gTexture->id);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures.bTexture->id);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures.blendMap->id);
}

void TerrainRenderer::LoadModelMatrix(const Terrain& terrain)
{
	glm::mat4 transformation = Maths::CreateModelMatrix
	(
		glm::vec3(terrain.position.x, 0.0f, terrain.position.y),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.0f
	);
	shader.LoadModelMatrix(transformation);
}

void TerrainRenderer::UnbindTerrain()
{
	glBindVertexArray(0);
}