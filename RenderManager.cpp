#include "RenderManager.h"

#include <GL/glew.h>
#include <fmt/format.h>

#include "Window.h"
#include "GL.h"
#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Entities::Entity;
using Entities::PointLight;
using Entities::Camera;
using Entities::Skybox;
using Entities::Player;
using Waters::WaterTile;
using Waters::WaterFrameBuffers;
using Engine::Settings;

RenderManager::RenderManager()
    : m_iblRenderer(m_converterShader, m_convolutionShader, m_preFilterShader, m_brdfShader),
      m_iblMaps(m_iblRenderer),
      m_instances(std::make_shared<InstanceBuffer>()),
      m_matrices(std::make_shared<MatrixBuffer>()),
      m_lights(std::make_shared<LightsBuffer>()),
      m_shared(std::make_shared<SharedBuffer>()),
      m_instancedRenderer(m_gShader, m_fastInstancedShader, m_shadowShader, m_omniShadowShader, m_spotShadowShader, m_iblMaps, m_instances),
      m_lightRenderer(m_lightShader, m_shadowMap, m_pointShadowMap, m_spotShadowMap, m_gBuffer, m_iblMaps),
      m_bloomRenderer(m_downSampleShader, m_upSampleShader, m_lightingBuffer, m_bloomBuffer),
      m_postRenderer(m_postShader, m_lightingBuffer, m_bloomBuffer),
      m_skyboxRenderer(m_skyboxShader),
      m_waterRenderer(m_waterShader, m_waterFBOs),
      m_skybox(m_iblMaps.cubeMap),
      m_glVendor(GL::GetString(GL_VENDOR)),
      m_glRenderer(GL::GetString(GL_RENDERER)),
      m_glVersion(GL::GetString(GL_VERSION)),
      m_glslVersion(GL::GetString(GL_SHADING_LANGUAGE_VERSION)),
      m_isGPUMemoryInfo(GLEW::GetExtension("GL_NVX_gpu_memory_info")),
      m_currentFBO(m_waterFBOs.reflectionFBO->colorTextures[0])
{
    // Get settings
    const auto& settings = Settings::GetInstance();

    // Memory info check
    if (m_isGPUMemoryInfo)
    {
        GLint total   = GL::GetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
        m_totalMemory = static_cast<f32>(total) / 1024.0f;
    }

    // Load matrices
    m_matrices->LoadProjection(glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE));
    m_shared->LoadResolution(settings.window.dimensions, NEAR_PLANE, FAR_PLANE);
}

void RenderManager::BeginFrame
(
    EntityVec& entities,
    const DirLights& dirLights,
    const PointLights& pointLights,
    const SpotLights& spotLights,
    Player& player
)
{
    // Process entities
    ProcessEntities(entities);
    ProcessEntity(player);
    // Load directional lights
    m_lights->LoadDirectionalLights(dirLights);
    // Load point lights
    m_lights->LoadPointLights(pointLights);
    // Load spot lights
    m_lights->LoadSpotLights(spotLights);
}

void RenderManager::EndFrame()
{
    // Clear internal render data
    m_entities.clear();
    // Update ImGui windows
    RenderImGui();
}

// TODO: Add frustum culling to shadows
void RenderManager::RenderShadows(const Camera& camera, const glm::vec3& lightPos)
{
    // Bind shadow map
    m_shadowMap.BindShadowFBO();
    // Update cascades
    m_shadowMap.Update(camera, lightPos);

    // Peter-panning fix
    glCullFace(GL_FRONT);
    // Cascade clipping fix
    glEnable(GL_DEPTH_CLAMP);

    // Clear FBO
    Clear(GL_DEPTH_BUFFER_BIT);
    // Render entities
    m_instancedRenderer.Render(m_entities, Mode::Shadow);

    // Reset
    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_CLAMP);

    // Unbind shadow map
    m_shadowMap.BindDefaultFBO();
}

// TODO: Add frustum culling to shadows
void RenderManager::RenderPointShadows(const PointLights& lights)
{
    // Bind shadow map
    m_pointShadowMap.BindShadowCubeMap();

    // Clear FBO
    Clear(GL_DEPTH_BUFFER_BIT);

    for (usize i = 0; i < lights.size(); ++i)
    {
        // Update shadow map
        m_pointShadowMap.Update(i, lights[i].position);
        // Render entities
        m_instancedRenderer.Render(m_entities, Mode::OmniShadow);
    }

    // Unbind shadow map
    m_pointShadowMap.BindDefaultFBO();
}

// TODO: Add frustum culling to shadows
void RenderManager::RenderSpotShadows(const SpotLights& lights)
{
    // Bind shadow map
    m_spotShadowMap.BindShadowFBO();

    // Peter-panning fix
    glCullFace(GL_FRONT);
    // Cascade clipping fix
    glEnable(GL_DEPTH_CLAMP);

    // Clear depth
    Clear(GL_DEPTH_BUFFER_BIT);

    // Update each light
    for (usize i = 0; i < lights.size(); ++i)
    {
        // Update cascades
        m_spotShadowMap.Update(i, lights[i].position);
        // Render entities
        m_instancedRenderer.Render(m_entities, Mode::SpotShadow);
    }

    // Reset
    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_CLAMP);

    // Unbind shadow map
    m_shadowMap.BindDefaultFBO();
}

void RenderManager::RenderWaters(const WaterTiles& waters)
{
    // Bind FBO
    m_lightingBuffer.BindLightingBuffer();
    // Render waters
    m_waterShader.Start();
    m_waterRenderer.Render(waters);
    m_waterShader.Stop();
    // Unbind FBO
    m_lightingBuffer.BindDefaultFBO();
}

// TODO: Render multiple FBOs for water tiles
void RenderManager::RenderWaterFBOs(const WaterTiles& waters, Camera& camera)
{
    // Enable clip plane 0
    glEnable(GL_CLIP_DISTANCE0);

    // Reflection pass
    m_waterFBOs.BindReflection();
    // Move the camera
    f32 distance = 2.0f * (camera.position.y - waters[0].transform.position.y);
    camera.position.y -= distance;
    camera.InvertPitch();
    // Render
    RenderWaterScene(camera, glm::vec4(0.0f, 1.0f, 0.0f, -waters[0].transform.position.y));
    // Move it back to its original position
    camera.position.y += distance;
    camera.InvertPitch();

    // Refraction pass
    m_waterFBOs.BindRefraction();
    RenderWaterScene(camera, glm::vec4(0.0f, -1.0f, 0.0f, waters[0].transform.position.y));

    // Disable clip plane 0
    glDisable(GL_CLIP_DISTANCE0);
    // Bind default FBO
    m_waterFBOs.BindDefaultFBO();
}

void RenderManager::RenderGBuffer(const Camera& camera)
{
    // Cull entities
    CullEntities(camera);

    // Enable stencil
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // Bind GBuffer
    m_gBuffer.BindGBuffer();

    // Clear all bits
    glStencilMask(0xFF);
    // Clear FBO
    Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Set stencil parameters
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    // Load data
    m_matrices->LoadView(camera);
    m_shared->LoadCameraPos(camera);
    // Render
    m_instancedRenderer.Render(m_culledEntities, Mode::Deferred);

    // Unbind GBuffer
    m_gBuffer.BindDefaultFBO();
    // Disable stencil
    glDisable(GL_STENCIL_TEST);
}

void RenderManager::RenderLighting(const Camera& camera)
{
    // Disable depth test
    glDisable(GL_DEPTH_TEST);
    // Enable stencil
    glEnable(GL_STENCIL_TEST);

    // Bind FBO
    m_lightingBuffer.BindLightingBuffer();
    // Clear all bits
    glStencilMask(0xFF);
    // Clear FBO
    Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Copy depth
    CopyGBuffer(m_lightingBuffer.buffer, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Bind FBO again
    m_lightingBuffer.BindLightingBuffer();
    // Set stencil parameters
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    // Load data
    m_matrices->LoadView(camera);
    m_shared->LoadCameraPos(camera);
    // Do lighting pass
    m_lightShader.Start();
    m_lightRenderer.Render();
    m_lightShader.Stop();

    // Disable stencil
    glDisable(GL_STENCIL_TEST);
    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
    // Unbind FBO
    m_lightingBuffer.BindDefaultFBO();
}

void RenderManager::RenderBloom()
{
    // Disable depth test
    glDisable(GL_DEPTH_TEST);
    // Bind frame buffer
    m_bloomBuffer.BindBloomBuffer();

    // Start down shader
    m_downSampleShader.Start();
    // Render down samples
    m_bloomRenderer.RenderDownSamples();
    // Stop down shader
    m_downSampleShader.Stop();

    // Enable additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    // Start up shader
    m_upSampleShader.Start();
    // Render up samples
    m_bloomRenderer.RenderUpSamples();
    // Stop up shader
    m_upSampleShader.Stop();

    // Unbind frame buffer
    m_bloomBuffer.BindDefaultFBO();
    // Disable blending
    glDisable(GL_BLEND);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Render ImGui pass
    m_bloomRenderer.RenderImGui();
}

void RenderManager::RenderPostProcess()
{
    // Disable depth test
    glDisable(GL_DEPTH_TEST);
    // Clear FBO
    Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Do post-processing pass
    m_postShader.Start();
    m_postRenderer.Render();
    m_postShader.Stop();
    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
    // Render ImGui pass
    m_postRenderer.RenderImGui();
}

void RenderManager::RenderSkybox()
{
    // Bind FBO
    m_lightingBuffer.BindLightingBuffer();
    // Render skybox
    RenderSkyboxScene();
    // Unbind FBO
    m_lightingBuffer.BindDefaultFBO();
}

void RenderManager::CopyGBuffer(FbPtr& drawBuffer, GLbitfield flags)
{
    // Bind buffers
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer.buffer->id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBuffer->id);
    // Copy depth
    glBlitFramebuffer
    (
        0,                        // Source X bound
        0,                        // Source Y bound
        m_gBuffer.buffer->width,  // Source width
        m_gBuffer.buffer->height, // Source height
        0,                        // Destination X bound
        0,                        // Destination Y bound
        drawBuffer->width,        // Destination width
        drawBuffer->height,       // Destination height
        flags,                    // Copy flags
        GL_NEAREST                // Filtering
    );
    // Unbind
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RenderManager::Clear(GLbitfield flags)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(flags);
}

void RenderManager::RenderWaterScene(const Camera& camera, const glm::vec4& clipPlane)
{
    // Cull entities
    CullEntities(camera);
    // Clear FBO
    Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Load data
    m_matrices->LoadView(camera);
    m_shared->LoadCameraPos(camera);
    m_shared->LoadClipPlane(clipPlane);
    // Render entities
    m_instancedRenderer.Render(m_culledEntities, Mode::Fast);
    // Render skybox
    RenderSkyboxScene();
}

void RenderManager::RenderSkyboxScene()
{
    // Since z / w will be 1.0f, we need to use GL_LEQUAL to avoid Z fighting
    glDepthFunc(GL_LEQUAL);
    // Disable depth writing for performance
    glDepthMask(GL_FALSE);
    m_skyboxShader.Start();
    m_skyboxRenderer.Render(m_skybox);
    m_skyboxShader.Stop();
    // Reset
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void RenderManager::ProcessEntity(Entity& entity)
{
    // Lookup model
    auto iter = m_entities.find(entity.model);

    // If a batch was found
    if (iter != m_entities.end())
    {
        // Add entity to existing batch
        iter->second.emplace_back(&entity);
    }
    else
    {
        // Create a new batch
        m_entities[entity.model] = {&entity};
    }
}

void RenderManager::ProcessEntities(EntityVec& entities)
{
    // For all entities
    for (auto& entity : entities)
    {
        // Process entity
        ProcessEntity(entity);
    }
}

void RenderManager::CullEntities(const Camera& camera)
{
    // Update frustum
    m_viewFrustum.UpdateView(camera);
    // Copy entities (may be slow)
    m_culledEntities = m_entities;

    // Loop over all pairs
    for (auto& [model, entities] : m_culledEntities)
    {
        // Find elements to be removed
        auto toRemove = std::remove_if(entities.begin(), entities.end(), [this] (const auto& entity)
        {
            // Return true if entity is not visible
            return !m_viewFrustum.IsVisible(*entity);
        });
        // Remove elements
        entities.erase(toRemove, entities.end());

        // If no entities remain
        if (entities.empty())
        {
            // Remove batch
            m_culledEntities.erase(model);
        }
    }
}

void RenderManager::RenderImGui()
{
    // Begin menu bar
    if (ImGui::BeginMainMenuBar())
    {
        // Begin renderer menu
        if (ImGui::BeginMenu("Renderer"))
        {
            // Display basic info
            ImGui::Text
            (
                "GPU Info:\n%s\n%s\n%s\n%s",
                m_glVendor.c_str(),
                m_glRenderer.c_str(),
                m_glVersion.c_str(),
                m_glslVersion.c_str()
            );

            // If memory info is available
            if (m_isGPUMemoryInfo)
            {
                // Calculate available memory (MB)
                GLint available       = GL::GetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
                f32   availableMemory = static_cast<f32>(available) / 1024.0f;
                // Display
                ImGui::Text("Available | Total:\n%.2f MB | %.2f MB", availableMemory, m_totalMemory);
            }

            // End menu
            ImGui::EndMenu();
        }

        // Begin FBO selector menu (This kinda sucks, but it works)
        if (ImGui::BeginMenu("FBO Viewer"))
        {
            // Water reflection buffer
            if (ImGui::Button("WaterReflection"))
            {
                m_currentFBO = m_waterFBOs.reflectionFBO->colorTextures[0];
            }

            // Water refraction buffer
            if (ImGui::Button("WaterRefraction"))
            {
                m_currentFBO = m_waterFBOs.refractionFBO->colorTextures[0];
            }

            // Normal + Ambient Occlusion + Roughness buffer
            if (ImGui::Button("GNormal"))
            {
                m_currentFBO = m_gBuffer.buffer->colorTextures[0];
            }

            // Albedo + Metallic buffer
            if (ImGui::Button("GAlbedo"))
            {
                m_currentFBO = m_gBuffer.buffer->colorTextures[1];
            }

            // Emmisive buffer
            if (ImGui::Button("GEmmisive"))
            {
                m_currentFBO = m_gBuffer.buffer->colorTextures[2];
            }

            // Depth buffer
            if (ImGui::Button("GDepth"))
            {
                m_currentFBO = m_gBuffer.buffer->depthTexture;
            }

            // Accumulated lighting buffer (HDR)
            if (ImGui::Button("Lighting"))
            {
                m_currentFBO = m_lightingBuffer.buffer->colorTextures[0];
            }

            // Bloom buffer (HDR)
            if (ImGui::Button("Bloom"))
            {
                m_currentFBO = m_bloomBuffer.mipChain[0];
            }

            // End menu
            ImGui::EndMenu();
        }

        // End menu bar
        ImGui::EndMainMenuBar();
    }

    // Current FBO widget
    if (ImGui::Begin("CurrentFBO"))
    {
        // Use a child window for better scaling
        if (ImGui::BeginChild("Current"))
        {
            // Invert Y
            ImGui::Image
            (
                static_cast<ImTextureID>(m_currentFBO->id),
                ImGui::GetWindowSize(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
            // End child window
            ImGui::EndChild();
        }
    }

    // End widget
    ImGui::End();
}