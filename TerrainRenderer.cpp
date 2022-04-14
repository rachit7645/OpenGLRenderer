#include "TerrainRenderer.h"

using namespace Renderer;
using namespace Shader;
using namespace Terrains;

TerrainRenderer::TerrainRenderer(Shader::TerrainShader& sh) : shader { sh } {}

void TerrainRenderer::Render(std::vector<Terrain>& terrains)
{
	for (auto& terrain : terrains)
	{
		PrepareTerrain(terrain);
		LoadModelMatrix(terrain);
		glDrawElements(GL_TRIANGLES, terrain.vertex_count, GL_UNSIGNED_INT, static_cast<const void*>(0));
		UnbindTerrain();
	}
}

void TerrainRenderer::PrepareTerrain(Terrain& terrain)
{
	glBindVertexArray(terrain.vao.id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shader.LoadShineVariables(terrain.shineDamper, terrain.reflectivity);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain.texture->id);
}

void TerrainRenderer::LoadModelMatrix(Terrain& terrain)
{
	glm::mat4 trans_matrix = Maths::CreateTransformationMatrix(
		glm::vec3(terrain.grid_position.x, 0.0f, terrain.grid_position.y),
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