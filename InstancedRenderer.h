#ifndef INSTANCED_RENDERER_H
#define INSTANCED_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "Entity.h"
#include "Model.h"
#include "InstanceBuffer.h"
#include "FastInstancedShader.h"
#include "RenderConstants.h"
#include "ShadowShader.h"
#include "IBLMaps.h"
#include "GBufferShader.h"
#include "OmniShadowShader.h"
#include "SpotShadowShader.h"
#include "TextureBuffer.h"
#include "VertexPool.h"
#include "DrawCall.h"
#include "VertexBuffer.h"

namespace Renderer
{
    class InstancedRenderer
    {
    public:
        // Usings
        using BufferPtr      = std::shared_ptr<InstanceBuffer>;
        using HandlesPtr     = std::shared_ptr<TextureBuffer>;
        using MdPtr          = std::shared_ptr<Model>;
        using VBO            = std::shared_ptr<VertexBuffer>;
        using EntityVector   = std::vector<Entities::Entity*>;
        using TextureVector  = std::vector<MeshTextures>;
        using DrawCallVector = std::vector<DrawCall>;
        using Batch          = std::unordered_map<MdPtr, EntityVector>;

        // Main constructor
        InstancedRenderer
        (
            Shader::GBufferShader& gShader,
            Shader::FastInstancedShader& fastShader,
            Shader::ShadowShader& shadowShader,
            Shader::OmniShadowShader& omniShadowShader,
            Shader::SpotShadowShader& spotShadowShader,
            Renderer::IBLMaps& iblMaps,
            VertexPool& vertexPool,
            BufferPtr instances,
            HandlesPtr handles
        );

        // Shaders
        Shader::GBufferShader&       gShader;          // Geometry Shader
        Shader::FastInstancedShader& fastShader;       // Fast Shader
        Shader::ShadowShader&        shadowShader;     // Directional Shadow Shader
        Shader::OmniShadowShader&    omniShadowShader; // Omnidirectional Shadow Shader
        Shader::SpotShadowShader&    spotShadowShader; // Spot Shadow Shader

        // Other data
        Renderer::IBLMaps& iblMaps;    // IBL data
        VertexPool&        vertexPool; // Vertex data pool
        BufferPtr          instances;  // Instance data
        HandlesPtr         handles;    // Texture handles

        // Render batch
        void Render(const Batch& batch, Mode mode);
    private:
        // Indirect buffer
        VBO indirectBuffer = nullptr;

        // Begin rendering
        void BeginRender(Mode mode);
        // End rendering
        void EndRender(Mode mode);

        // Load instance data
        void LoadInstanceData(const Batch& batch);
        // Load render data
        void LoadData(const Batch& batch, Mode mode);
        // Load textures
        static void LoadTextures(TextureVector& textures, const MdPtr& model);

        // Prepare indirect draw data
        usize CreateDrawCalls(const Batch& batch);
        // Load draw call data
        static void LoadDrawCalls(const std::vector<Renderer::DrawCall>& drawCalls);
    };
}

#endif
