#ifndef LIGHTING_RENDERER_H
#define LIGHTING_RENDERER_H

#include <memory>

#include "LightingShader.h"
#include "VertexArray.h"
#include "GBuffer.h"
#include "ShadowMap.h"
#include "IBLMaps.h"
#include "OmniShadowMap.h"

namespace Renderer
{
    class LightingRenderer
    {
    public:
        // Usings
        using VAO = std::shared_ptr<VertexArray>;

        // Main constructor
        LightingRenderer
        (
            Shader::LightingShader& shader,
            Renderer::ShadowMap& shadowMap,
            Renderer::OmniShadowMap& pointShadowMap,
            Renderer::GBuffer& gBuffer,
            Renderer::IBLMaps& iblMaps
        );

        // Data
        Shader::LightingShader&  shader;
        Renderer::ShadowMap&     shadowMap;
        Renderer::OmniShadowMap& pointShadowMap;
        Renderer::GBuffer&       gBuffer;
        Renderer::IBLMaps&       iblMaps;

        // Render light pass
        void Render();
    private:
        // Quad VAO
        VAO m_vao;
    };
}

#endif
