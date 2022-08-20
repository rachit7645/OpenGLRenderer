#include "WaterRenderer.h"

#include "GLM.h"
#include "Maths.h"

using namespace Renderer;

using Waters::WaterTile;
using Waters::WaterFrameBuffers;

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
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void WaterRenderer::Render(const std::vector<WaterTile>& waters, const WaterFrameBuffers& waterFBOs)
{
	Prepare(waterFBOs);
	for (const auto& water : waters)
	{
		PrepareWater(water);
		glDrawArrays(GL_TRIANGLES, 0,vao->vertexCount);
	}
	Unbind();
}

void WaterRenderer::Prepare(const WaterFrameBuffers& waterFBOs)
{
	glBindVertexArray(vao->id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.reflectionFBO->colorTexture->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.refractionFBO->colorTexture->id);
}

void WaterRenderer::PrepareWater(const WaterTile& water)
{
	glm::mat4 matrix = Maths::CreateModelMatrixTS
	(
		water.position,
		Waters::WATER_TILE_SIZE
	);
	shader.LoadModelMatrix(matrix);
}

void WaterRenderer::Unbind()
{
	glBindVertexArray(0);
}
