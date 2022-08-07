#include "WaterRenderer.h"

#include "GLM.h"
#include "Maths.h"

using namespace Renderer;

const std::vector<f32> TILE_VERTICES =
{
	-1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f
};

WaterRenderer::WaterRenderer(Shader::WaterShader& shader)
	: shader(shader),
	  vao(std::make_shared<VertexArray>(2, TILE_VERTICES))
{
}

void WaterRenderer::Render(const std::vector<Waters::WaterTile>& waters)
{
	glBindVertexArray(vao->id);
	for (const auto& tile : waters)
	{
		glm::mat4 matrix = Maths::CreateModelMatrixTS
		(
			tile.position,
			Waters::WATER_TILE_SIZE
		);
		shader.LoadModelMatrix(matrix);
		glDrawArrays(GL_TRIANGLES, 0,vao->vertexCount);
	}
	glBindVertexArray(0);
}
