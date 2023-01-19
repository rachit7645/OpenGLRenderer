#include "WaterRenderer.h"

#include "Maths.h"
#include "Util.h"

using namespace Renderer;

using Waters::WaterTile;
using Waters::WaterFrameBuffers;

constexpr auto WATER_WAVE_SPEED = 0.04f;

WaterRenderer::WaterRenderer(Shader::WaterShader& shader, Waters::WaterFrameBuffers& waterFBOs)
	: shader(shader),
	  waterFBOs(waterFBOs)
{
	// Water tile vertices
	const std::vector<f32> TILE_VERTICES =
	{
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f
	};

	// Create water tile VAO
	m_vao = std::make_shared<VertexArray>(2, TILE_VERTICES);

	// Connect texture units
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void WaterRenderer::Render(const std::vector<WaterTile>& waters)
{
	// Prepare renderer
	Prepare();
	// For each water
	for (const auto& water : waters)
	{
		// Prepare water
		PrepareWater(water);
		// Render water
		glDrawArrays(GL_TRIANGLES, 0, m_vao->vertexCount);
	}
	// Unbind renderer
	Unbind();
}

void WaterRenderer::Prepare()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Bind framebuffers
	waterFBOs.reflectionFBO->colorTextures[0]->Bind(0);
	waterFBOs.refractionFBO->colorTextures[0]->Bind(1);
	// Load dudv move factor
	m_moveFactor += WATER_WAVE_SPEED * g_Delta;
	m_moveFactor = std::fmod(m_moveFactor, 1.0f);
	shader.LoadMoveFactor(m_moveFactor);
}

void WaterRenderer::PrepareWater(const WaterTile& water)
{
	// Bind maps
	water.dudvMap->Bind(2);
	water.normalMap->Bind(3);
	// Create model matrix
	auto matrix = Maths::CreateModelMatrixTS
	(
		water.position,
		Waters::WATER_TILE_SIZE
	);
	// Load model matrix
	shader.LoadModelMatrix(matrix);
}

void WaterRenderer::Unbind()
{
	// Unbind VAO
	glBindVertexArray(0);
}
