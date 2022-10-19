#include "LightingRenderer.h"

#include <vector>

using namespace Renderer;

using Shader::LightingShader;

const std::vector<f32> QUAD_VERTICES =
{
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f
};

LightingRenderer::LightingRenderer
(
	LightingShader& shader,
	Renderer::ShadowMap& shadowMap,
	GBuffer& gBuffer
)
	: shader(shader),
	  shadowMap(shadowMap),
	  gBuffer(gBuffer),
	  m_vao(std::make_shared<VertexArray>(2, QUAD_VERTICES))
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void LightingRenderer::Render()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Activate global buffer textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[0]->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[1]->id);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[2]->id);
	// Activate shadow map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMap.buffer->depthTexture->id);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}
