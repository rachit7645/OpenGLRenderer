#include "SSAORenderer.h"

// Using namespaces
using namespace Renderer;

// Usings
using Shader::SSAOShader;
using Shader::SSAOBlurShader;

SSAORenderer::SSAORenderer
(
	SSAOShader& shader,
	SSAOBlurShader& blurShader,
	SSAOBuffers& buffers,
	GBuffer& gBuffer
)
	: shader(shader),
	  blurShader(blurShader),
	  buffers(buffers),
	  gBuffer(gBuffer)
{
	// Vertex data
	const std::vector<f32> QUAD_VERTICES =
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f
	};

	// Create full screen quad
	m_vao = std::make_shared<VertexArray>(2, QUAD_VERTICES);

	// Bind SSAO texture units
	shader.Start();
	shader.ConnectTextureUnits();

	// Bind blur texture units
	blurShader.Start();
	blurShader.ConnectTextureUnits();
}

void SSAORenderer::RenderSSAO()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Activate gNormal
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[0]->id);
	// Activate gDepth
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->depthTexture->id);
	// Activate noise texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, buffers.noiseTexture->id);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}

void SSAORenderer::RenderSSAOBlur()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Activate SSAO input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffers.ssaoBuffer->colorTextures[0]->id);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}