#include "RenderManager.h"

#include <GL/glew.h>
#include <fmt/format.h>

#include "Maths.h"
#include "Window.h"
#include "GL.h"
#include "Settings.h"

using namespace Renderer;

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
	  m_instancedRenderer(m_fastInstancedShader, m_shadowInstancedShader, m_shadowMap, m_iblMaps, m_instances),
	  m_gRenderer(m_gShader, m_instances),
	  m_lightRenderer(m_lightShader, m_shadowMap, m_gBuffer, m_iblMaps),
	  m_postRenderer(m_postShader, m_lightingBuffer, m_bloomBuffer),
	  m_bloomRenderer(m_downSampleShader, m_upSampleShader, m_lightingBuffer, m_bloomBuffer),
	  m_skyboxRenderer(m_skyboxShader),
	  m_waterRenderer(m_waterShader, m_waterFBOs),
	  m_skybox(m_iblMaps.cubeMap),
	  m_matrices(std::make_shared<MatrixBuffer>()),
	  m_lights(std::make_shared<LightsBuffer>()),
	  m_shared(std::make_shared<SharedBuffer>()),
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

void RenderManager::RenderShadows(const Camera& camera, const glm::vec3& lightPos)
{
	// Update cascades
	m_shadowMap.Update(camera, lightPos);
	// Peter-panning fix
	glCullFace(GL_FRONT);
	// Cascade clipping fix
	glEnable(GL_DEPTH_CLAMP);
	// Bind shadow map
	m_shadowMap.BindShadowFBO();
	// Clear FBO
	GL::ClearDepth(m_shadowMap.buffer->id, 1.0f);
	// Render
	RenderShadowScene(camera);
	// Unbind shadow map
	m_shadowMap.BindDefaultFBO();
	// Reset
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_CLAMP);
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

	// Clear reflection FBO
	GL::ClearColor(m_waterFBOs.reflectionFBO->id, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	GL::ClearDepth(m_waterFBOs.reflectionFBO->id, 1.0f);
	// Clear refraction FBO
	GL::ClearColor(m_waterFBOs.refractionFBO->id, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	GL::ClearDepth(m_waterFBOs.refractionFBO->id, 1.0f);

	// Reflection pass
	m_waterFBOs.BindReflection();
	// Move the camera
	f32 distance = 2.0f * (camera.position.y - waters[0].position.y);
	camera.position.y -= distance;
	camera.InvertPitch();
	// Render
	RenderWaterScene(camera, glm::vec4(0.0f, 1.0f, 0.0f, -waters[0].position.y));
	// Move it back to its original position
	camera.position.y += distance;
	camera.InvertPitch();

	// Refraction pass
	m_waterFBOs.BindRefraction();
	// Render
	RenderWaterScene(camera, glm::vec4(0.0f, -1.0f, 0.0f, waters[0].position.y));

	// Disable clip plane 0
	glDisable(GL_CLIP_DISTANCE0);
	// Bind default FBO
	m_waterFBOs.BindDefaultFBO();
}

void RenderManager::RenderGBuffer(const Camera& camera)
{
	// Enable stencil
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// Clear all bits
	glStencilMask(0xFF);
	// Clear FBO
	GL::ClearColor(m_gBuffer.buffer->id, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	GL::ClearDepthStencil(m_gBuffer.buffer->id, 1.0f, 0x0);
	// Set stencil parameters
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	// Load data
	m_matrices->LoadView(camera);
	m_shared->LoadCameraPos(camera);
	// Bind GBuffer
	m_gBuffer.BindGBuffer();
	// Render
	m_gRenderer.Render(m_entities);
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
	// Clear all bits
	glStencilMask(0xFF);
	// Clear FBO
	GL::ClearColor(m_lightingBuffer.buffer->id, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	GL::ClearDepthStencil(m_lightingBuffer.buffer->id, 1.0f, 0x0);
	// Copy depth
	CopyDepth();
	// Set stencil parameters
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	// Load data
	m_matrices->LoadView(camera);
	m_shared->LoadCameraPos(camera);
	// Bind FBO again
	m_lightingBuffer.BindLightingBuffer();
	// Do lighting pass
	m_lightShader.Start();
	m_lightRenderer.Render();
	m_lightShader.Stop();
	// Unbind FBO
	m_lightingBuffer.BindDefaultFBO();
	// Disable stencil
	glDisable(GL_STENCIL_TEST);
	// Re-enable depth test
	glEnable(GL_DEPTH_TEST);
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
}

void RenderManager::RenderPostProcess()
{
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	// Clear FBO
	GL::ClearColor(GL::DEFAULT_FRAME_BUFFER, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	GL::ClearDepth(GL::DEFAULT_FRAME_BUFFER, 1.0f);
	// Do post-processing pass
	m_postShader.Start();
	m_postRenderer.Render();
	m_postShader.Stop();
	// Re-enable depth test
	glEnable(GL_DEPTH_TEST);
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

void RenderManager::CopyDepth()
{
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Copy depth
	glBlitNamedFramebuffer
	(
		m_gBuffer.buffer->id,                        // Source
		m_lightingBuffer.buffer->id,                 // Destination
		0, 0,                                        // Source X & Y
		settings.window.dimensions.x,                // Source Width
		settings.window.dimensions.y,                // Source Height
		0, 0,                                        // Destination X & Y
		settings.window.dimensions.x,                // Destination Width
		settings.window.dimensions.y,                // Destination Height
		GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, // Mask
		GL_NEAREST                                   // Filter
	);
}

void RenderManager::RenderWaterScene(const Camera& camera, const glm::vec4& clipPlane)
{
	// Load data
	m_matrices->LoadView(camera);
	m_shared->LoadCameraPos(camera);
	m_shared->LoadClipPlane(clipPlane);
	// Render entities
	m_instancedRenderer.Render(m_entities, Mode::Fast);
	// Render skybox
	RenderSkyboxScene();
}

void RenderManager::RenderShadowScene(const Camera& camera)
{
	// Load data
	m_matrices->LoadView(camera);
	m_shared->LoadCameraPos(camera);
	// Render entities
	m_instancedRenderer.Render(m_entities, Mode::Shadow);
}

void RenderManager::RenderSkyboxScene()
{
	// Since z / w will be 1.0f, we need to use GL_LEQUAL to avoid Z fighting
	glDepthFunc(GL_LEQUAL);
	// Disable depth writing for performance
	glDepthMask(GL_FALSE);
	// Render skybox
	m_skyboxShader.Start();
	m_skyboxRenderer.Render(m_skybox);
	m_skyboxShader.Stop();
	// Reset state
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void RenderManager::ProcessEntity(Entity& entity)
{
	// Try to see if a batch exists
	auto iter = m_entities.find(entity.model);

	// If batch exists
	if (iter != m_entities.end())
	{
		// Add entity to the batch
		iter->second.emplace_back(&entity);
	}
	// If the batch does not exist
	else
	{
		// Create one
		m_entities[entity.model] = {&entity};
	}
}

void RenderManager::ProcessEntities(EntityVec& entities)
{
	// For each entity
	for (auto& entity : entities)
	{
		// Process Entity
		ProcessEntity(entity);
	}
}

// This kinda sucks, but it works
void RenderManager::RenderImGui()
{
	// If menu bar is visible
	if (ImGui::BeginMainMenuBar())
	{
		// if menu is visible
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

			// If GPU memory information is available
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

		// If FBO viewer is visible
		if (ImGui::BeginMenu("FBO Viewer"))
		{
			// If button was clicked
			if (ImGui::Button("WaterReflection"))
			{
				// Set FBO
				m_currentFBO = m_waterFBOs.reflectionFBO->colorTextures[0];
			}

			// If button was clicked
			if (ImGui::Button("WaterRefraction"))
			{
				// Set FBO
				m_currentFBO = m_waterFBOs.refractionFBO->colorTextures[0];
			}

			// If button was clicked
			if (ImGui::Button("GNormal"))
			{
				// Set FBO
				m_currentFBO = m_gBuffer.buffer->colorTextures[0];
			}

			// If button was clicked
			if (ImGui::Button("GAlbedo"))
			{
				// Set FBO
				m_currentFBO = m_gBuffer.buffer->colorTextures[1];
			}

			// If button was clicked
			if (ImGui::Button("GEmmisive"))
			{
				// Set FBO
				m_currentFBO = m_gBuffer.buffer->colorTextures[2];
			}

			// If button was clicked
			if (ImGui::Button("GMaterial"))
			{
				// Set FBO
				m_currentFBO = m_gBuffer.buffer->colorTextures[3];
			}

			// If button was clicked
			if (ImGui::Button("GDepth"))
			{
				// Set FBO
				m_currentFBO = m_gBuffer.buffer->depthTexture;
			}

			// If button was clicked
			if (ImGui::Button("Lighting"))
			{
				// Set FBO
				m_currentFBO = m_lightingBuffer.buffer->colorTextures[0];
			}

			// If button was clicked
			if (ImGui::Button("Bloom"))
			{
				// Set FBO
				m_currentFBO = m_bloomBuffer.mipChain[0];
			}

			// Close menu
			ImGui::EndMenu();
		}

		// End main menu
		ImGui::EndMainMenuBar();
	}

	// If current FBO displayer is visible
	if (ImGui::Begin("CurrentFBO"))
	{
		// If child window is visible
		if (ImGui::BeginChild("Current"))
		{
			// Show selected FBO
			ImGui::Image
			(
				reinterpret_cast<ImTextureID>(m_currentFBO->id),
				ImGui::GetWindowSize(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
			// End child window
			ImGui::EndChild();
		}
	}

	// End imgui
	ImGui::End();
}