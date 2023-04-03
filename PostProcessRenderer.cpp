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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightingBuffer.buffer->colorTextures[0]->id);
	// Bind bloom buffer
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomBuffer.mipChain[0]->id);
	// Load bloom strength
	shader.LoadBloomStrength(m_bloomStrength);
	// Render quad
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	// Unbind vao
	glBindVertexArray(0);
}

void PostProcessRenderer::RenderImGui()
{
	// If main menu bar is visible
	if (ImGui::BeginMainMenuBar())
	{
		// If bloom menu is visible
		if (ImGui::BeginMenu("Bloom"))
		{
			// Bloom strength
			ImGui::Text("Strength:");
			ImGui::DragFloat("##bstr", &m_bloomStrength, 0.00125f, 0.0f, 1.0f, "%.4f");
			// End menu
			ImGui::EndMenu();
		}
		// End menu bar
		ImGui::EndMainMenuBar();
	}
}