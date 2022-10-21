#include "MasterRenderer.h"

#include <GL/glew.h>

#include "Maths.h"
#include "Window.h"

using namespace Renderer;

using Entities::Entity;
using Entities::Light;
using Entities::Camera;
using Entities::Skybox;
using Entities::Player;
using Waters::WaterTile;
using Waters::WaterFrameBuffers;

MasterRenderer::MasterRenderer()
	: m_instances(std::make_shared<InstanceBuffer>()),
	  instancedRenderer(fastInstancedShader, shadowInstancedShader, m_shadowMap, m_instances),
	  gRenderer(gShader, m_instances),
	  lightRenderer(lightShader, m_shadowMap, m_gBuffer),
	  skyboxRenderer(skyboxShader),
	  guiRenderer(guiShader),
	  waterRenderer(waterShader, m_waterFBOs),
	  m_matrices(std::make_shared<MatrixBuffer>()),
	  m_lights(std::make_shared<LightsBuffer>()),
	  m_shared(std::make_shared<SharedBuffer>())
{
	m_matrices->LoadProjection(glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE));
	m_shared->LoadSkyColor(GL_SKY_COLOR);
	m_shared->LoadFarPlane(FAR_PLANE);
}

void MasterRenderer::BeginFrame
(
	std::vector<Entity>& entities,
	const std::vector<Light>& lights,
	Player& player
)
{
	ProcessEntities(entities);
	ProcessEntity(player);

	m_lights->LoadLights(lights);
}

void MasterRenderer::RenderScene(const Camera& camera, Mode mode, const glm::vec4& clipPlane)
{
	Prepare(camera, clipPlane);
	RenderEntities(mode);

	if (mode == Mode::Fast)
	{
		RenderSkybox();
	}
}

void MasterRenderer::EndFrame()
{
	// Clear internal render data
	m_entities.clear();
	// Update ImGui windows
	RenderImGui();
}

void MasterRenderer::Prepare(const Camera& camera, const glm::vec4& clipPlane)
{
	glClearColor(GL_CLEAR_COLOR.r, GL_CLEAR_COLOR.g, GL_CLEAR_COLOR.b, GL_CLEAR_COLOR.a);
	glClear(GL_CLEAR_FLAGS);

	m_matrices->LoadView(camera);
	m_shared->LoadClipPlane(clipPlane);
	m_shared->LoadCameraPos(camera);
}

void MasterRenderer::RenderEntities(Mode mode)
{
	instancedRenderer.Render(m_entities, mode);
}

void MasterRenderer::RenderSkybox()
{
	// Since z / w will be 1.0f, we need to use GL_LEQUAL to avoid Z fighting
	glDepthFunc(GL_LEQUAL);
	// Disable depth writing for performance
	glDepthMask(GL_FALSE);
	skyboxShader.Start();
	skyboxRenderer.Render(m_skybox);
	skyboxShader.Stop();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void MasterRenderer::RenderGUIs(const std::vector<GUI>& guis)
{
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	guiShader.Start();
	guiRenderer.Render(guis);
	guiShader.Stop();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void MasterRenderer::RenderWaters(const std::vector<WaterTile>& waters)
{
	waterShader.Start();
	waterRenderer.Render(waters);
	waterShader.Stop();
}

// TODO: Render multiple FBOs for water tiles
void MasterRenderer::RenderWaterFBOs(const std::vector<WaterTile>& waters,Camera& camera)
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

void MasterRenderer::RenderShadows(const Camera& camera, const Light& light)
{
	m_shadowMap.BindShadowFBO();
	m_shadowMap.Update(camera, light.position);
	glCullFace(GL_FRONT);
	RenderScene(camera, Mode::Shadow, glm::vec4(0.0f));
	glCullFace(GL_BACK);
	m_shadowMap.BindDefaultFBO();
}

void MasterRenderer::RenderGBuffer(const Camera& camera)
{
	m_gBuffer.BindGBuffer();
	Prepare(camera);
	gRenderer.Render(m_entities);
	m_gBuffer.BindDefaultFBO();
}

void MasterRenderer::RenderLighting(const Camera& camera)
{
	Prepare(camera);
	lightShader.Start();
	lightRenderer.Render();
	lightShader.Stop();
}

void MasterRenderer::RenderImGui()
{
	static auto current = m_waterFBOs.reflectionFBO->colorTextures[0];

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Renderer"))
		{
			if (ImGui::Button("WaterReflection"))
			{
				current = m_waterFBOs.reflectionFBO->colorTextures[0];
			}

			if (ImGui::Button("WaterRefraction"))
			{
				current = m_waterFBOs.refractionFBO->colorTextures[0];
			}

			if (ImGui::Button("GPosition"))
			{
				current = m_gBuffer.buffer->colorTextures[0];
			}

			if (ImGui::Button("GNormal"))
			{
				current = m_gBuffer.buffer->colorTextures[1];
			}

			if (ImGui::Button("GAlbedoSpec"))
			{
				current = m_gBuffer.buffer->colorTextures[2];
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

void MasterRenderer::CopyDepth()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer.buffer->id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

	glBlitFramebuffer
	(
		0, 0,
		Window::DIMENSIONS.x,
		Window::DIMENSIONS.y,
		0, 0,
		Window::DIMENSIONS.x,
		Window::DIMENSIONS.y,
		GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
	);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MasterRenderer::ProcessEntity(Entity& entity)
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

void MasterRenderer::ProcessEntities(std::vector<Entity>& entities)
{
	for (auto& entity : entities)
	{
		ProcessEntity(entity);
	}
}