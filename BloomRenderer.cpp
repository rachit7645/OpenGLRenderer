#include "BloomRenderer.h"

#include <vector>

#include "Util.h"

// Using namespaces
using namespace Renderer;

// Usings
using Shader::DownSampleShader;
using Shader::UpSampleShader;

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

    // Set up shader
    upShader.Start();
    upShader.ConnectTextureUnits();
    upShader.Stop();
}

void BloomRenderer::RenderDownSamples()
{
    // Bind vao
    glBindVertexArray(m_vao->id);
    // Bind lighting buffer
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lightingBuffer.buffer->colorTextures[0]->id);
    // Set first sample flag
    downShader.SetIsFirstSample(true);

    // For each mip map
    for (usize i = 0; i < bloomBuffer.mipChain.size(); ++i)
    {
        // Check first sample flag
        if (i == 1)
        {
            downShader.SetIsFirstSample(false);
        }
        // Get mip
        auto& mip = bloomBuffer.mipChain[i];
        // Set view port
        glViewport(0, 0, mip->width, mip->height);
        // Attach to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mip->type, mip->id, 0);
        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Render quad
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
        // Set current mip as texture input
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mip->id);
    }

    // Unbind vao
    glBindVertexArray(0);
}

void BloomRenderer::RenderUpSamples()
{
    // Bind vao
    glBindVertexArray(m_vao->id);
    // Load bloom radius
    upShader.LoadFilterRadius(m_bloomRadius);

    for (usize i = bloomBuffer.mipChain.size() - 1; i > 0; --i)
    {
        // Get textures
        auto& mip     = bloomBuffer.mipChain[i];
        auto& nextMip = bloomBuffer.mipChain[i - 1];
        // Bind source texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mip->id);
        // Set view port
        glViewport(0, 0, nextMip->width, nextMip->height);
        // Attach to FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, nextMip->type, nextMip->id, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
    }

    // Unbind vao
    glBindVertexArray(0);
}

void BloomRenderer::RenderImGui()
{
    // If main menu bar is visible
    if (ImGui::BeginMainMenuBar())
    {
        // If bloom menu is visible
        if (ImGui::BeginMenu("Bloom"))
        {
            // Bloom radius
            ImGui::Text("Radius:");
            ImGui::DragFloat("##brad", &m_bloomRadius, 0.0005f, 0.0f, 0.1f, "%.4f");
            // End menu
            ImGui::EndMenu();
        }
        // End menu bar
        ImGui::EndMainMenuBar();
    }
}