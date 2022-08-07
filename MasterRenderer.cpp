#include "MasterRenderer.h"

using namespace Renderer;

using Entities::Entity;
using Entities::Light;
using Entities::Camera;
using Entities::Skybox;
using Terrains::Terrain;

MasterRenderer::MasterRenderer() 
	: renderer(shader),
	  terrainRenderer(terrainShader),
	  skyboxRenderer(skyboxShader),
	  guiRenderer(guiShader),
	  m_matrices(std::make_shared<MatrixBuffer>()),
	  m_lights(std::make_shared<LightsBuffer>())
{
	m_matrices->LoadProjection(glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE));
}

void MasterRenderer::Prepare()
{
	glClearColor(GL_CLEAR_COLOR.r, GL_CLEAR_COLOR.g, GL_CLEAR_COLOR.b, GL_CLEAR_COLOR.a);
	glClear(GL_CLEAR_FLAGS);
}

void MasterRenderer::Update(const std::vector<Light>& lights, const Camera& camera)
{
	m_matrices->LoadView(camera);
	m_lights->LoadLight(lights);
}

void MasterRenderer::Render(const std::vector<Light>& lights, const Camera& camera)
{
	Prepare();
	Update(lights, camera);

	RenderEntities();
	RenderTerrains();
	RenderSkybox();
	RenderGUIs();

	m_entities.clear();
	m_terrains.clear();
	m_guis.clear();
}

void MasterRenderer::RenderEntities()
{
	shader.Start();
	shader.LoadSkyColour(GL_CLEAR_COLOR);
	renderer.Render(m_entities);
	shader.Stop();
}

void MasterRenderer::RenderTerrains()
{
	terrainShader.Start();
	terrainShader.LoadSkyColour(GL_CLEAR_COLOR);
	terrainRenderer.Render(m_terrains);
	terrainShader.Stop();
}

void MasterRenderer::RenderSkybox()
{
	// Since z / w will be 1.0f, we need to use GL_LEQUAL to avoid Z fighting
	glDepthFunc(GL_LEQUAL);
	// Disable depth writing for performance
	glDepthMask(GL_FALSE);
	skyboxShader.Start();
	skyboxShader.LoadFogColor(GL_CLEAR_COLOR);
	skyboxRenderer.Render(m_skybox);
	skyboxShader.Stop();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void MasterRenderer::RenderGUIs()
{
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	guiShader.Start();
	guiRenderer.Render(m_guis);
	guiShader.Stop();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void MasterRenderer::ProcessEntity(const Entities::Entity& entity)
{
	auto iter = m_entities.find(entity.model);
	if (iter != m_entities.end())
	{
		iter->second.push_back(entity);
	}
	else
	{
		m_entities[entity.model] = { entity };
	}
}

void MasterRenderer::ProcessEntities(const std::vector<Entity>& entities)
{
	for (const auto& entity : entities)
	{
		ProcessEntity(entity);
	}
}

void MasterRenderer::ProcessTerrain(const Terrain& terrain)
{
	m_terrains.push_back(terrain);
}

void MasterRenderer::ProcessTerrains(const std::vector<Terrain>& terrains)
{
	m_terrains = terrains;
}

void MasterRenderer::ProcessGUIs(const std::vector<Renderer::GUI>& guis)
{
	m_guis = guis;
}
