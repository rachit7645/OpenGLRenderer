#include "WaterRenderer.h"

#include "GLM.h"
#include "Maths.h"
#include "Util.h"

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
	// Bind vao
	glBindVertexArray(vao->id);
	// Bind framebuffers
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.reflectionFBO->colorTexture->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.refractionFBO->colorTexture->id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.refractionFBO->depthTexture->id);
	// Load dudv move factor
	moveFactor += WATER_WAVE_SPEED * g_Delta;
	moveFactor = std::fmod(moveFactor, 1.0f);
	shader.LoadMoveFactor(moveFactor);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::PrepareWater(const WaterTile& water)
{
	// Bind maps
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, water.dudvMap->id);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, water.normalMap->id);
	// Load model matrix
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
	glDisable(GL_BLEND);
}
