#include "SSAORenderer.h"

#include <vector>

using namespace Renderer;

using Shader::SSAOShader;

const std::vector<f32> QUAD_VERTICES =
{
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f
};

SSAORenderer::SSAORenderer
(
	SSAOShader& shader,
	SSAOBuffers& ssaoBuffers,
	GBuffer& gBuffer
)
	: shader(shader),
	  ssaoBuffers(ssaoBuffers),
	  gBuffer(gBuffer),
	  m_vao(std::make_shared<VertexArray>(2, QUAD_VERTICES))
{
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void SSAORenderer::RenderSSAO()
{
	// Start shader
	shader.Start();
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Activate global buffer textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[0]->id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[1]->id);
	// Activate noise buffer
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ssaoBuffers.noiseBuffer->id);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
	// Stop shader
	shader.Stop();
}