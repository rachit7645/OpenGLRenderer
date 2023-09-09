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

namespace Renderer
{
    class InstancedRenderer
    {
    public:
        // Usings
        using BufferPtr     = std::shared_ptr<InstanceBuffer>;
        using HandlesPtr    = std::shared_ptr<TextureBuffer>;
        using MdPtr         = std::shared_ptr<Model>;
        using EntityVector  = std::vector<Entities::Entity*>;
        using TextureVector = std::vector<MeshTextures>;
        using Batch         = std::unordered_map<MdPtr, EntityVector>;

        // Main constructor
        InstancedRenderer
        (
            Shader::GBufferShader& gShader,
            Shader::FastInstancedShader& fastShader,
            Shader::ShadowShader& shadowShader,
            Shader::OmniShadowShader& omniShadowShader,
            Shader::SpotShadowShader& spotShadowShader,
            Renderer::IBLMaps& iblMaps,
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
        Renderer::IBLMaps& iblMaps;   // IBL data
        BufferPtr          instances; // Instance data
        HandlesPtr         handles;   // Texture handles

        // Render batch
        void Render(const Batch& batch, Mode mode);
    private:
        // Begin rendering
        void BeginRender(Mode mode);
        // End rendering
        void EndRender(Mode mode);
        // Load instance data
        void LoadInstanceData(const EntityVector& entities);
        // Prepare mesh
        static void PrepareMesh(const Mesh& mesh, Mode mode);
        // Load textures
        static void LoadTextures(TextureVector& textures, const MdPtr& model);
        // Unbind mesh
        static void UnbindMesh();
        // Load render data
        void LoadData(const Batch& batch, Mode mode);
    };
}

#endif
