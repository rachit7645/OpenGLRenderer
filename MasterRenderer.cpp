#include "MasterRenderer.h"

#include <GL/glew.h>

#include "Maths.h"

using namespace Renderer;

using Entities::Entity;
using Entities::Light;
using Entities::Camera;
using Entities::Skybox;
using Entities::Player;
using Waters::WaterTile;

MasterRenderer::MasterRenderer(Renderer::ShadowFrameBuffer& shadowFBO)
	: instancedRenderer(instancedShader, fastInstancedShader, shadowInstancedShader, shadowFBO),
	  skyboxRenderer(skyboxShader),
	  guiRenderer(guiShader),
	  waterRenderer(waterShader),
	  m_matrices(std::make_shared<MatrixBuffer>()),
	  m_lights(std::make_shared<LightsBuffer>()),
	  m_shared(std::make_shared<SharedBuffer>())
{
	m_matrices->LoadProjection(glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE));
	m_matrices->LoadLightProjection(Maths::CreateOrthoMatrix(0.1f, 50.0f, 20.0f));
	m_shared->LoadSkyColor(GL_SKY_COLOR);
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
	m_matrices->LoadLightView(Maths::CreateLookAtMatrix(lights[0].position));
}

void MasterRenderer::RenderScene(const Camera& camera, const glm::vec4& clipPlane, Mode mode)
{
	Prepare(camera, clipPlane);
	RenderEntities(mode);

	if (mode != Mode::Shadow)
	{
		RenderSkybox();
	}
}

void MasterRenderer::EndFrame()
{
	// Clear internal render data
	m_entities.clear();
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

void MasterRenderer::RenderWaters(const std::vector<WaterTile>& waters, const Waters::WaterFrameBuffers& waterFBOs)
{
	waterShader.Start();
	waterRenderer.Render(waters, waterFBOs);
	waterShader.Stop();
}

void MasterRenderer::RenderShadows(const ShadowFrameBuffer& shadowFBO, const Camera& camera)
{
	shadowFBO.BindShadowFBO();
	glCullFace(GL_FRONT);
	RenderScene(camera, glm::vec4(0.0f), Mode::Shadow);
	glCullFace(GL_BACK);
	shadowFBO.BindDefaultFBO();
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