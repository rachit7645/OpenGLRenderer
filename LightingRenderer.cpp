#include "LightingRenderer.h"

#include <vector>

using namespace Renderer;

using Shader::LightingShader;

LightingRenderer::LightingRenderer
(
    LightingShader& shader,
    ShadowMap& shadowMap,
    OmniShadowMap& pointShadowMap,
    Renderer::SpotShadowMap& spotShadowMap,
    GBuffer& gBuffer,
    IBLMaps& iblMaps
)
    : shader(shader),
      shadowMap(shadowMap),
      pointShadowMap(pointShadowMap),
      spotShadowMap(spotShadowMap),
      gBuffer(gBuffer),
      iblMaps(iblMaps)
{
    // Quad vertices
    const std::vector<f32> QUAD_VERTICES =
    {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f
    };

    // Create full screen quad
    m_vao = std::make_shared<VertexArray>(2, QUAD_VERTICES);

    // Connect texture slots
    shader.Start();
    shader.ConnectTextureUnits();
    shader.Stop();
}

void LightingRenderer::Render()
{
    // Bind vao
    glBindVertexArray(m_vao->id);

    // Activate gNormal
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[0]->id);

    // Activate gAlbedo
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[1]->id);

    // Activate gEmmisive
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->colorTextures[2]->id);

    // Activate gDepth
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gBuffer.buffer->depthTexture->id);

    // Activate irradiance map
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.irradiance->id);

    // Activate pre-filter map
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.preFilter->id);

    // Activate BRDF LUT map
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, iblMaps.brdfLut->id);

    // Activate shadow map
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMap.buffer->depthTexture->id);

    // Activate point shadow map
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, pointShadowMap.shadowCubeMap->depthTexture->id);

    // Activate point shadow map
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, spotShadowMap.buffer->depthTexture->id);

    // Render quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);

    // Unbind vao
    glBindVertexArray(0);
}