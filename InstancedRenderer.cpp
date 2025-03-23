#include "InstancedRenderer.h"

// Using namespaces
using namespace Renderer;

// Usings
using Shader::GBufferShader;
using Shader::FastInstancedShader;
using Shader::ShadowShader;
using Shader::OmniShadowShader;
using Shader::SpotShadowShader;

InstancedRenderer::InstancedRenderer
(
    Shader::GBufferShader& gShader,
    FastInstancedShader& fastShader,
    ShadowShader& shadowShader,
    OmniShadowShader& omniShadowShader,
    SpotShadowShader& spotShadowShader,
    IBLMaps& iblMaps,
    BufferPtr instances
)
    : gShader(gShader),
      fastShader(fastShader),
      shadowShader(shadowShader),
      omniShadowShader(omniShadowShader),
      spotShadowShader(spotShadowShader),
      iblMaps(iblMaps),
      instances(std::move(instances))
{
    // Connect geometry shader texture units
    gShader.Start();
    gShader.ConnectTextureUnits();
    gShader.Stop();

    // Connect fast shader texture units
    fastShader.Start();
    fastShader.ConnectTextureUnits();
    fastShader.Stop();
}

void InstancedRenderer::Render(const Batch& batch, Mode mode)
{
    // Begin render pass
    BeginRender(mode);
    // For each pair
    for (const auto& [model, entities] : batch)
    {
        // Load instance data
        LoadData(entities);
        // For each mesh
        for (const auto& mesh : model->meshes)
        {
            // Prepare mesh
            PrepareMesh(mesh, mode);
            // Draw instances
            glDrawElementsInstanced
            (
                GL_TRIANGLES,
                mesh.vao->vertexCount,
                GL_UNSIGNED_INT,
                nullptr,
                static_cast<GLint>(entities.size())
            );
            // Unbind mesh
            UnbindMesh();
        }
    }
    // End render pass
    EndRender(mode);
}

void InstancedRenderer::BeginRender(Mode mode)
{
    // Bind instance SSBO
    instances->Bind();

    // Select mode
    switch (mode)
    {
    case Mode::Deferred:
        // Start geometry shader
        gShader.Start();
        break;

    case Mode::Fast:
        // Start fast shader
        fastShader.Start();
        // Activate irradiance map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.irradiance->id);
        // Activate pre-filter map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, iblMaps.preFilter->id);
        // Activate BRDF LUT map
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, iblMaps.brdfLut->id);
        break;

    case Mode::Shadow:
        // Start shadow shader
        shadowShader.Start();
        break;

    case Mode::OmniShadow:
        // Start omni shadow shader
        omniShadowShader.Start();
        break;

    case Mode::SpotShadow:
        // Start spot shadow shader
        spotShadowShader.Start();
        break;
    }
}

void InstancedRenderer::EndRender(Mode mode)
{
    // Unbind instance SSBO
    instances->Unbind();

    // Select mode
    switch (mode)
    {
    case Mode::Deferred:
        // Stop geometry shader
        gShader.Stop();
        break;

    case Mode::Fast:
        // Stop fast shader
        fastShader.Stop();
        break;

    case Mode::Shadow:
        // Stop shadow shader
        shadowShader.Stop();
        break;

    case Mode::OmniShadow:
        // Stop omni shadow shader
        omniShadowShader.Stop();
        break;

    case Mode::SpotShadow:
        // Stop spot shadow shader
        spotShadowShader.Stop();
        break;
    }
}

void InstancedRenderer::LoadData(const EntityVector& entities)
{
    // Load data
    instances->LoadInstanceData(entities);
}

void InstancedRenderer::PrepareMesh(const Mesh& mesh, Mode mode)
{
    // Bind VAO
    glBindVertexArray(mesh.vao->id);

    // Select mode
    switch (mode)
    {
    case Mode::Deferred:
        // Activate albedo
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.albedo->id);
        // Activate normal
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.normal->id);
        // Activate ambient occlusion, metallic and roughness
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.aoRghMtl->id);
        // Activate emmisive
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.emmisive->id);
        break;

    case Mode::Fast:
        // Load albedo
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.albedo->id);
        // Load aoMtlRgh
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mesh.textures.aoRghMtl->id);
        break;

    case Mode::Shadow:
    case Mode::OmniShadow:
    case Mode::SpotShadow:
        // Nothing to do here
        break;
    }
}

void InstancedRenderer::UnbindMesh()
{
    // Unbind
    glBindVertexArray(0);
}