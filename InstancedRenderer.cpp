#include "InstancedRenderer.h"
#include "Log.h"

// Using namespaces
using namespace Renderer;

// Usings
using Shader::GBufferShader;
using Shader::FastInstancedShader;
using Shader::ShadowShader;
using Shader::OmniShadowShader;
using Shader::SpotShadowShader;

// Aliases
using DrawCallVector = InstancedRenderer::DrawCallVector;

InstancedRenderer::InstancedRenderer
(
    Shader::GBufferShader& gShader,
    FastInstancedShader& fastShader,
    ShadowShader& shadowShader,
    OmniShadowShader& omniShadowShader,
    SpotShadowShader& spotShadowShader,
    IBLMaps& iblMaps,
    VertexPool& vertexPool,
    BufferPtr instances,
    HandlesPtr handles
)
    : gShader(gShader),
      fastShader(fastShader),
      shadowShader(shadowShader),
      omniShadowShader(omniShadowShader),
      spotShadowShader(spotShadowShader),
      iblMaps(iblMaps),
      vertexPool(vertexPool),
      instances(std::move(instances)),
      handles(std::move(handles)),
      indirectBuffer(std::make_shared<VertexBuffer>())
{
    // Connect geometry shader texture units
    gShader.Start();
    gShader.ConnectTextureUnits();

    // Connect fast shader texture units
    fastShader.Start();
    fastShader.ConnectTextureUnits();
    fastShader.Stop();

    // Unbind VAO
    glBindVertexArray(0);
    // Create buffer
    indirectBuffer->CreateBuffer();
    indirectBuffer->Bind(GL_DRAW_INDIRECT_BUFFER);
    indirectBuffer->AllocateMemory(GL_DRAW_INDIRECT_BUFFER, MAX_DRAW_CALLS * sizeof(DrawCall));
    indirectBuffer->Unbind(GL_DRAW_INDIRECT_BUFFER);
}

void InstancedRenderer::Render(const Batch& batch, Mode mode)
{
    // Begin render pass
    BeginRender(mode);

    // Load textures
    LoadData(batch, mode);
    // Load instance data
    LoadInstanceData(batch);
    // Create draw calls
    auto nDrawCalls = CreateDrawCalls(batch);

    // Draw
    glMultiDrawElementsIndirect
    (
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        reinterpret_cast<const void*>(0),
        static_cast<GLsizei>(nDrawCalls),
        0
    );

    // End render pass
    EndRender(mode);
}

void InstancedRenderer::BeginRender(Mode mode)
{
    // Bind instance SSBO
    instances->Bind();
    // Bind VAO
    glBindVertexArray(vertexPool.poolVAO->id);
    // Bind indirect VBO
    indirectBuffer->Bind(GL_DRAW_INDIRECT_BUFFER);

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
    // Unbind VAO
    glBindVertexArray(0);
    // Bind indirect VBO
    indirectBuffer->Unbind(GL_DRAW_INDIRECT_BUFFER);

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

void InstancedRenderer::LoadData(const Batch& batch, Mode mode)
{
    // Check if we are in a shadow mode
    if (mode != Mode::Deferred && mode != Mode::Fast)
    {
        // No textures are needed for shadow modes
        return;
    }

    // Create texture vector
    TextureVector textureSets = {};
    textureSets.reserve(Renderer::SHADER_MAX_TEXTURE_SETS); // Inefficient, but IDC (I kinda do)

    // Loop over all models in batch
    for (const auto& [model, _] : batch)
    {
        // Load textures
        LoadTextures(textureSets, model);
    }

    // Upload textures
    handles->LoadTextures(textureSets);
}

void InstancedRenderer::LoadTextures(TextureVector& textures, const MdPtr& model)
{
    // Get meshes
    const auto& meshes = model->meshes;

    // For each mesh
    for (const auto& mesh : meshes)
    {
        // Add texture to vector
        textures.emplace_back(mesh.textures);
    }
}

void InstancedRenderer::LoadInstanceData(const Batch& batch)
{
    // Load data
    instances->LoadInstanceData(batch);
}

usize InstancedRenderer::CreateDrawCalls(const Batch& batch)
{
    // Create draw call vector
    DrawCallVector drawCalls = {};

    for (const auto& [model, entities] : batch)
    {
        // For every mesh
        for (const Mesh& mesh : model->meshes)
        {
            // Get mesh specification
            const auto& spec = vertexPool.specs[mesh.id];

            // Initialise draw call
            DrawCall call = {};
            // Add parameters
            call.count = static_cast<GLuint>(spec.vertexCount);
            call.instanceCount = static_cast<GLuint>(entities.size());
            call.firstIndex = static_cast<GLuint>(spec.indexOffset);
            call.baseVertex = static_cast<GLint>(spec.vertexOffset);
            call.baseInstance = 0;

            // Add to vector
            drawCalls.emplace_back(call);
        }
    }

    // Load draw calls to buffer
    LoadDrawCalls(drawCalls);

    // Return vector
    return drawCalls.size();
}

void InstancedRenderer::LoadDrawCalls(const std::vector<DrawCall>& drawCalls)
{
    // Get size in bytes
    usize sizeBytes = drawCalls.size() * sizeof(DrawCall);
    // Get pointer
    auto ptr = glMapBufferRange
    (
        GL_DRAW_INDIRECT_BUFFER,
        0,
        static_cast<GLsizeiptr>(sizeBytes),
        GL_MAP_WRITE_BIT
    );
    // Copy
    std::memcpy(ptr, drawCalls.data(), sizeBytes);
    // Unmap pointer
    glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
}