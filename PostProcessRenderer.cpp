#include "PostProcessRenderer.h"

#include <vector>

// Using namespaces
using namespace Renderer;

// Usings
using Shader::PostProcessShader;

PostProcessRenderer::PostProcessRenderer
(
	PostProcessShader& shader,
	LightingBuffer& lightingBuffer,
	BloomBuffer& bloomBuffer
)
	: shader(shader),
	  lightingBuffer(lightingBuffer),
	  bloomBuffer(bloomBuffer)
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

	// Bind texture units
	shader.Start();
	shader.ConnectTextureUnits();
	shader.Stop();
}

void PostProcessRenderer::Render()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Bind lighting buffer
	lightingBuffer.buffer->colorTextures[0]->Bind(0);
	// Bind bloom buffer
	bloomBuffer.mipChain[0]->Bind(1);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}
