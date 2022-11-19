#include "RenderManager.h"

#include <GL/glew.h>
#include <fmt/format.h>

#include "Maths.h"
#include "Window.h"
#include "GL.h"

using namespace Renderer;

using Entities::Entity;
using Entities::Light;
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
	  m_shared(std::make_shared<SharedBuffer>())
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

void RenderManager::BeginFrame(EntityVec& entities, const Lights& lights, Player& player)
{
	ProcessEntities(entities);
	ProcessEntity(player);

	m_lights->LoadLights(lights);
}

void RenderManager::EndFrame()
{
	// Clear internal render data
	m_entities.clear();
	// Update ImGui windows
	RenderImGui();
}

void RenderManager::RenderShadows(const Camera& camera, const Light& light)
{
	m_shadowMap.BindShadowFBO();
	m_shadowMap.Update(camera, light.position);
	glCullFace(GL_FRONT);
	RenderScene(camera, Mode::Shadow, glm::vec4(0.0f));
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
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer.buffer->id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

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
	static auto current = m_waterFBOs.reflectionFBO->colorTextures[0];

	if (ImGui::BeginMainMenuBar())
	{
		// TODO: Move FPS to the same menu as gpu info
		if (ImGui::BeginMenu("Renderer"))
		{
			ImGui::Text
			(
				fmt::format
				(
					"GPU Info:\n{}\n{}\n{}\n{}",
					GL::GetString(GL_VENDOR),
					GL::GetString(GL_RENDERER),
					GL::GetString(GL_VERSION),
					GL::GetString(GL_SHADING_LANGUAGE_VERSION)
				).c_str()
			);

			if (glewGetExtension("GL_NVX_gpu_memory_info"))
			{
				ImGui::Text
				(
					fmt::format
					(
						"Available | Total:\n{:.2f} MB | {:.2f} MB",
						GL::GetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX) / 1024.0,
						GL::GetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX)   / 1024.0
					).c_str()
				);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("FBO Viewer"))
		{
			if (ImGui::Button("WaterReflection"))
			{
				current = m_waterFBOs.reflectionFBO->colorTextures[0];
			}

			if (ImGui::Button("WaterRefraction"))
			{
				current = m_waterFBOs.refractionFBO->colorTextures[0];
			}

			if (ImGui::Button("GNormal"))
			{
				current = m_gBuffer.buffer->colorTextures[0];
			}

			if (ImGui::Button("GAlbedo"))
			{
				current = m_gBuffer.buffer->colorTextures[1];
			}

			if (ImGui::Button("GNormalMap"))
			{
				current = m_gBuffer.buffer->colorTextures[2];
			}

			if (ImGui::Button("GDepth"))
			{
				current = m_gBuffer.buffer->depthTexture;
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
				reinterpret_cast<ImTextureID>(current->id),
				ImGui::GetWindowSize(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
			ImGui::EndChild();
		}
	}

	ImGui::End();
}