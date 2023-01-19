#include "BloomRenderer.h"

#include <vector>

#include "Util.h"
#include "GL.h"
#include "GLM.h"

// Using namespaces
using namespace Renderer;

// Usings
using Shader::DownSampleShader;
using Shader::UpSampleShader;

// Constants
constexpr f32 BLOOM_RADIUS = 0.005f;

BloomRenderer::BloomRenderer
(
	DownSampleShader& downShader,
	UpSampleShader& upShader,
	LightingBuffer& lightingBuffer,
	BloomBuffer& bloomBuffer
)
	: downShader(downShader),
	  upShader(upShader),
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

	// Set down shader
	downShader.Start();
	downShader.ConnectTextureUnits();
	downShader.Stop();

	// Set up shader
	upShader.Start();
	upShader.ConnectTextureUnits();
	upShader.LoadFilterRadius(BLOOM_RADIUS);
	upShader.Stop();
}

void BloomRenderer::RenderDownSamples()
{
	// Bind vao
	glBindVertexArray(m_vao->id);
	// Bind lighting buffer
	lightingBuffer.buffer->colorTextures[0]->Bind(0);
	// Set first sample flag
	downShader.SetIsFirstSample(true);

	// For each mip map
	for (usize i = 0; i < bloomBuffer.mipChain.size(); ++i)
	{
		// Check first sample flag
		if (i == 1)
		{
			// Load uniform
			downShader.SetIsFirstSample(false);
		}
		// Get mip
		auto& mip = bloomBuffer.mipChain[i];
		// Set view port
		glViewport(0, 0, mip->width, mip->height);
		// Attach to FBO
		glNamedFramebufferTexture(bloomBuffer.buffer->id, GL_COLOR_ATTACHMENT0, mip->id, 0);
		// Clear
		GL::ClearColor(bloomBuffer.buffer->id, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		// Render quad
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
		// Set current mip as texture input
		mip->Bind(0);
	}

	// Unbind vao
	glBindVertexArray(0);
}

void BloomRenderer::RenderUpSamples()
{
	// Bind vao
	glBindVertexArray(m_vao->id);

	for (usize i = bloomBuffer.mipChain.size() - 1; i > 0; --i)
	{
		// Get textures
		auto& mip     = bloomBuffer.mipChain[i];
		auto& nextMip = bloomBuffer.mipChain[i - 1];
		// Bind source texture
		mip->Bind(0);
		// Set view port
		glViewport(0, 0, nextMip->width, nextMip->height);
		// Attach to FBO
		glNamedFramebufferTexture(bloomBuffer.buffer->id, GL_COLOR_ATTACHMENT0, nextMip->id, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	}

	// Unbind vao
	glBindVertexArray(0);
}