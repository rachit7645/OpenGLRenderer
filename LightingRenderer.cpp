#include "LightingRenderer.h"

#include <vector>

using namespace Renderer;

using Shader::LightingShader;

LightingRenderer::LightingRenderer
(
	LightingShader& shader,
	ShadowMap& shadowMap,
	GBuffer& gBuffer,
	IBLMaps& iblMaps
)
	: shader(shader),
	  shadowMap(shadowMap),
	  gBuffer(gBuffer),
	  iblMaps(iblMaps)
{
	const std::vector<f32> QUAD_VERTICES =
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f
	};

	m_vao = std::make_shared<VertexArray>(2, QUAD_VERTICES);

	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void LightingRenderer::Render()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Activate gPosition
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[0]->id);
	// Activate gNormal
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[1]->id);
	// Activate gAlbedo
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[2]->id);
	// Activate gNormalMap
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[3]->id);
	// Activate shadow map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMap.buffer->depthTexture->id);
	// Active irradiance map
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.irradiance->id);
	// Active pre-filter map
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.preFilter->id);
	// Active BRDF LUT map
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, iblMaps.brdfLut->id);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}
