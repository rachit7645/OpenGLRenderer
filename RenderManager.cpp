#include "RenderManager.h"

#include <GL/glew.h>
#include <fmt/format.h>

#include "Maths.h"
#include "Window.h"
#include "GL.h"

// Utility defines

#ifndef GL_AVAILABLE_MEMORY
#define GL_AVAILABLE_MEMORY GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX
#endif

#ifndef GL_TOTAL_MEMORY
#define GL_TOTAL_MEMORY GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX
#endif

using namespace Renderer;

using Entities::Entity;
using Entities::PointLight;
using Entities::Camera;
using Entities::Skybox;
using Entities::Player;
using Waters::WaterTile;
using Waters::WaterFrameBuffers;

RenderManager::RenderManager()
	: m_instances(std::make_shared<InstanceBuffer>()),
	  m_instancedRenderer(m_fastInstancedShader, m_shadowInstancedShader, m_shadowMap, m_instances),
	  m_gRenderer(m_gShader, m_instances),
	  m_lightRenderer(m_lightShader, m_shadowMap, m_gBuffer, m_iblMaps),
	  m_skyboxRenderer(m_skyboxShader),
	  m_guiRenderer(m_guiShader),
	  m_waterRenderer(m_waterShader, m_waterFBOs),
	  m_skybox(m_iblMaps.cubeMap),
	  m_matrices(std::make_shared<MatrixBuffer>()),
	  m_lights(std::make_shared<LightsBuffer>()),
	  m_shared(std::make_shared<SharedBuffer>()),
	  m_glVendor(GL::GetString(GL_VENDOR)),
	  m_glRenderer(GL::GetString(GL_RENDERER)),
	  m_glVersion(GL::GetString(GL_VERSION)),
	  m_glslVersion(GL::GetString(GL_SHADING_LANGUAGE_VERSION)),
	  isGPUMemoryInfo(glewGetExtension("GL_NVX_gpu_memory_info")),
	  totalMemory(static_cast<f32>(GL::GetIntegerv(GL_TOTAL_MEMORY)) / 1024.0f),
	  m_currentFBO(m_waterFBOs.reflectionFBO->colorTextures[0])
{
	m_matrices->LoadProjection(glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE));
	m_shared->LoadResolution(Window::WINDOW_DIMENSIONS, NEAR_PLANE, FAR_PLANE);

	// Dump shaders
	m_fastInstancedShader.DumpToFile("dumps/FIS.s");
	m_shadowInstancedShader.DumpToFile("dumps/SIS.s");
	m_gShader.DumpToFile("dumps/GS.s");
	m_lightShader.DumpToFile("dumps/LS.s");
	m_skyboxShader.DumpToFile("dumps/SKB.s");
	m_guiShader.DumpToFile("dumps/GUI.s");
	m_waterShader.DumpToFile("dumps/WTR.s");
}

void RenderManager::BeginFrame
(
	EntityVec& entities,
	const DirLights& dirLights,
	const PointLights& pointLights,
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
	// Render
	RenderScene(camera, Mode::Shadow, glm::vec4(0.0f));
	// Reset
	glCullFace(GL_BACK);
	m_shadowMap.BindDefaultFBO();
}

void RenderManager::RenderWaters(const WaterTiles& waters)
{
	m_waterShader.Start();
	m_waterRenderer.Render(waters);
	m_waterShader.Stop();
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
	RenderScene(camera, Mode::Fast, glm::vec4(0.0f, 1.0f, 0.0f, -waters[0].position.y));
	// Move it back to its original position
	camera.position.y += distance;
	camera.InvertPitch();

	// Refraction pass
	m_waterFBOs.BindRefraction();
	RenderScene(camera, Mode::Fast, glm::vec4(0.0f, -1.0f, 0.0f, waters[0].position.y));

	// Disable clip plane 0
	glDisable(GL_CLIP_DISTANCE0);
	// Bind default FBO
	m_waterFBOs.BindDefaultFBO();
}

void RenderManager::RenderGBuffer(const Camera& camera)
{
	m_gBuffer.BindGBuffer();
	Prepare(camera);
	m_gRenderer.Render(m_entities);
	m_gBuffer.BindDefaultFBO();
}

void RenderManager::RenderLighting(const Camera& camera)
{
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	Prepare(camera);
	m_lightShader.Start();
	m_lightRenderer.Render();
	m_lightShader.Stop();
	// Re-enable depth test
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::RenderSkybox()
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

void RenderManager::RenderGUIs(const GUIs& guis)
{
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_guiShader.Start();
	m_guiRenderer.Render(guis);
	m_guiShader.Stop();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void RenderManager::CopyDepth()
{
	// Bind buffers
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer.buffer->id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	// Copy depth
	glBlitFramebuffer
	(
		0, 0,
		Window::WINDOW_DIMENSIONS.x,
		Window::WINDOW_DIMENSIONS.y,
		0, 0,
		Window::WINDOW_DIMENSIONS.x,
		Window::WINDOW_DIMENSIONS.y,
		GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
	);
	// Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		m_entities[entity.model] = { &entity };
	}
}

void RenderManager::ProcessEntities(EntityVec& entities)
{
	for (auto& entity : entities)
	{
		ProcessEntity(entity);
	}
}

void RenderManager::RenderScene(const Camera& camera, Mode mode, const glm::vec4& clipPlane)
{
	Prepare(camera, clipPlane);
	RenderEntities(mode);
	RenderSkybox();
}

void RenderManager::Prepare(const Camera& camera, const glm::vec4& clipPlane)
{
	glClearColor(GL_CLEAR_COLOR.r, GL_CLEAR_COLOR.g, GL_CLEAR_COLOR.b, GL_CLEAR_COLOR.a);
	glClear(GL_CLEAR_FLAGS);

	m_matrices->LoadView(camera);
	m_shared->LoadClipPlane(clipPlane);
	m_shared->LoadCameraPos(camera);
}

void RenderManager::RenderEntities(Mode mode)
{
	m_instancedRenderer.Render(m_entities, mode);
}

// This kinda sucks, but it works
void RenderManager::RenderImGui()
{
	if (ImGui::BeginMainMenuBar())
	{
		// TODO: Move FPS to the same menu as gpu info
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
			if (isGPUMemoryInfo)
			{
				// Calculate available memory (MB)
				GLint available       = GL::GetIntegerv(GL_AVAILABLE_MEMORY);
				f32   availableMemory = static_cast<f32>(available) / 1024.0f;
				// Display
				ImGui::Text("Available | Total:\n%.2f MB | %.2f MB", availableMemory, totalMemory);
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