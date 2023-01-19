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
	// Activate gNormal
	gBuffer.buffer->colorTextures[0]->Bind(0);
	// Activate gAlbedo
	gBuffer.buffer->colorTextures[1]->Bind(1);
	// Activate gEmmisive
	gBuffer.buffer->colorTextures[2]->Bind(2);
	// Activate gMaterial
	gBuffer.buffer->colorTextures[3]->Bind(3);
	// Activate gDepth
	gBuffer.buffer->depthTexture->Bind(4);
	// Activate shadow map
	shadowMap.buffer->depthTexture->Bind(5);
	// Activate irradiance map
	iblMaps.irradiance->Bind(6);
	// Activate pre-filter map
	iblMaps.preFilter->Bind(7);
	// Activate BRDF LUT map
	iblMaps.brdfLut->Bind(8);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}
