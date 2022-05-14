#include "TerrainRenderer.h"

using namespace Renderer;

using Shader::TerrainShader;
using Terrains::TerrainTextures;
using Terrains::Terrain;

TerrainRenderer::TerrainRenderer(TerrainShader& shaderRef) : shader { shaderRef }
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void TerrainRenderer::Render(const std::vector<Terrain>& terrains)
{
	for (auto& terrain : terrains)
	{
		PrepareTerrain(terrain);
		LoadModelMatrix(terrain);
		glDrawElements(GL_TRIANGLES, terrain.vertexCount, GL_UNSIGNED_INT, static_cast<const void*>(0));
		UnbindTerrain();
	}
}

void TerrainRenderer::PrepareTerrain(const Terrain& terrain)
{
	glBindVertexArray(terrain.vao->id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
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
	glm::mat4 trans_matrix = Maths::CreateTransformationMatrix
	(
		glm::vec3(terrain.gridPosition.x, 0.0f, terrain.gridPosition.y),
		glm::vec3(0.0f, 0.0f, 0.0f), 1.0f
	);
	shader.LoadTransformationMatrix(trans_matrix);
}

void TerrainRenderer::UnbindTerrain()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}