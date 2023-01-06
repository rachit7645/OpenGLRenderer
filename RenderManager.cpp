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
	  m_instancedRenderer(m_fastInstancedShader, m_shadowInstancedShader, m_shadowMap, m_instances),
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
	// Bind shadow map
	m_shadowMap.BindShadowFBO();
	// Update cascades
	m_shadowMap.Update(camera, lightPos);
	// Peter-panning fix
	glCullFace(GL_FRONT);
	// Cascade clipping fix
	glEnable(GL_DEPTH_CLAMP);
	// Render
	RenderShadowScene(camera);
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
	RenderWaterScene(camera, glm::vec4(0.0f, -1.0f, 0.0f, waters[0].position.y));

	// Disable clip plane 0
	glDisable(GL_CLIP_DISTANCE0);
	// Bind default FBO
	m_waterFBOs.BindDefaultFBO();
}

void RenderManager::RenderGBuffer(const Camera& camera)
{
	// Bind GBuffer
	m_gBuffer.BindGBuffer();
	// Clear FBO
	Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load data
	m_matrices->LoadView(camera);
	m_shared->LoadCameraPos(camera);
	// Render
	m_gRenderer.Render(m_entities);
	// Unbind GBuffer
	m_gBuffer.BindDefaultFBO();
}

void RenderManager::RenderLighting(const Camera& camera)
{
	// Bind FBO
	m_lightingBuffer.BindLightingBuffer();
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	// Clear FBO
	Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load data
	m_matrices->LoadView(camera);
	m_shared->LoadCameraPos(camera);
	// Do lighting pass
	m_lightShader.Start();
	m_lightRenderer.Render();
	m_lightShader.Stop();
	// Re-enable depth test
	glEnable(GL_DEPTH_TEST);
	// Copy depth
	CopyDepth();
	// Unbind FBO
	m_lightingBuffer.BindDefaultFBO();
}

void RenderManager::RenderBloom()
{
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	// Bind frame buffer
	m_bloomBuffer.BindBloomBuffer();
	// Clear FBO
	Clear(GL_COLOR_BUFFER_BIT);

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
	Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	// Bind buffers
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer.buffer->id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightingBuffer.buffer->id);
	// Copy depth
	glBlitFramebuffer
	(
		0, 0,
		settings.window.dimensions.x,
		settings.window.dimensions.y,
		0, 0,
		settings.window.dimensions.x,
		settings.window.dimensions.y,
		GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
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
	// Clear FBO
	Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	// Clear FBO
	Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	m_skyboxShader.Start();
	m_skyboxRenderer.Render(m_skybox);
	m_skyboxShader.Stop();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void RenderManager::ProcessEntity(Entity& entity)
{
	auto iter = m_entities.find(entity.model);

	if (iter != m_entities.end())
	{
		iter->second.emplace_back(&entity);
	}
	else
	{
		m_entities[entity.model] = {&entity};
	}
}

void RenderManager::ProcessEntities(EntityVec& entities)
{
	for (auto& entity : entities)
	{
		ProcessEntity(entity);
	}
}

// This kinda sucks, but it works
void RenderManager::RenderImGui()
{
	if (ImGui::BeginMainMenuBar())
	{
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

			// If available
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

		if (ImGui::BeginMenu("FBO Viewer"))
		{
			if (ImGui::Button("WaterReflection"))
			{
				m_currentFBO = m_waterFBOs.reflectionFBO->colorTextures[0];
			}

			if (ImGui::Button("WaterRefraction"))
			{
				m_currentFBO = m_waterFBOs.refractionFBO->colorTextures[0];
			}

			if (ImGui::Button("GNormal"))
			{
				m_currentFBO = m_gBuffer.buffer->colorTextures[0];
			}

			if (ImGui::Button("GAlbedo"))
			{
				m_currentFBO = m_gBuffer.buffer->colorTextures[1];
			}

			if (ImGui::Button("GMaterial"))
			{
				m_currentFBO = m_gBuffer.buffer->colorTextures[2];
			}

			if (ImGui::Button("GDepth"))
			{
				m_currentFBO = m_gBuffer.buffer->depthTexture;
			}

			if (ImGui::Button("Lighting"))
			{
				m_currentFBO = m_lightingBuffer.buffer->colorTextures[0];
			}

			if (ImGui::Button("Bloom"))
			{
				m_currentFBO = m_bloomBuffer.mipChain[0];
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("CurrentFBO"))
	{
		if (ImGui::BeginChild("Current"))
		{
			ImGui::Image
			(
				reinterpret_cast<ImTextureID>(m_currentFBO->id),
				ImGui::GetWindowSize(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
			ImGui::EndChild();
		}
	}

	ImGui::End();
}