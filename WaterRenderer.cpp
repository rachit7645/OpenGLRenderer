#include "WaterRenderer.h"

#include "Util.h"

using namespace Renderer;

using Waters::WaterTile;
using Waters::WaterFrameBuffers;

constexpr auto WATER_WAVE_SPEED = 0.04f;

WaterRenderer::WaterRenderer(Shader::WaterShader& shader, Waters::WaterFrameBuffers& waterFBOs)
	: shader(shader),
	  waterFBOs(waterFBOs)
{
	// Tile vertex data
	const std::vector<f32> TILE_VERTICES =
	{
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f
	};

	// Create tile VAO
	m_vao = std::make_shared<VertexArray>(2, TILE_VERTICES);

	// Connect texture units
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void WaterRenderer::Render(const std::vector<WaterTile>& waters)
{
	// Prepare render
	Prepare();
	// For all waters
	for (const auto& water : waters)
	{
		// Prepare water
		PrepareWater(water);
		// Draw water tile
		glDrawArrays(GL_TRIANGLES, 0, m_vao->vertexCount);
	}
	// Unbind render
	Unbind();
}

void WaterRenderer::Prepare()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Bind framebuffers
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.reflectionFBO->colorTextures[0]->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterFBOs.refractionFBO->colorTextures[0]->id);
	// Load dudv move factor
	m_moveFactor += WATER_WAVE_SPEED * g_Delta;
	m_moveFactor = std::fmod(m_moveFactor, 1.0f);
	shader.LoadMoveFactor(m_moveFactor);
}

void WaterRenderer::PrepareWater(const WaterTile& water)
{
	// Bind maps
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, water.dudvMap->id);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, water.normalMap->id);
	// Load model matrix
	shader.LoadModelMatrix(water.transform.GetModelMatrix());
}

void WaterRenderer::Unbind()
{
	// Unbind VAO
	glBindVertexArray(0);
}
