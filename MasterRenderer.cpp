#include "MasterRenderer.h"

using namespace Renderer;

using Entities::Entity;
using Entities::Light;
using Entities::Camera;
using Entities::Skybox;
using Terrains::Terrain;

MasterRenderer::MasterRenderer() : renderer(shader), terrainRenderer(terrainShader), skyboxRenderer(skyboxShader)
{
	glm::mat4 projection = glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	shader.Start();
	shader.LoadProjectionMatrix(projection);
	shader.Stop();

	terrainShader.Start();
	terrainShader.LoadProjectionMatrix(projection);
	terrainShader.Stop();

	skyboxShader.Start();
	skyboxShader.LoadProjectionMatrix(projection);
	skyboxShader.Stop();
}

void MasterRenderer::Prepare()
{
	glClearColor(GL_CLEAR_COLOR.r, GL_CLEAR_COLOR.g, GL_CLEAR_COLOR.b, GL_CLEAR_COLOR.a);
	glClear(GL_CLEAR_FLAGS);
}

void MasterRenderer::Render(const Light& light, const Camera& camera)
{
	Prepare();

	RenderEntities(light, camera);
	RenderTerrains(light, camera);
	RenderSkybox(camera);

	entities.clear();
	terrains.clear();
}

void MasterRenderer::RenderEntities(const Light& light, const Camera& camera)
{
	shader.Start();
	shader.LoadViewMatrix(camera);
	shader.LoadLight(light);
	shader.LoadSkyColour(GL_CLEAR_COLOR);
	renderer.Render(entities);
	shader.Stop();
}

void MasterRenderer::RenderTerrains(const Light& light, const Camera& camera)
{
	terrainShader.Start();
	terrainShader.LoadViewMatrix(camera);
	terrainShader.LoadLight(light);
	terrainShader.LoadSkyColour(GL_CLEAR_COLOR);
	terrainRenderer.Render(terrains);
	terrainShader.Stop();
}

void MasterRenderer::RenderSkybox(const Camera& camera)
{
	// Since z / w will be 1.0f, we need to use GL_LEQUAL to avoid Z fighting
	glDepthFunc(GL_LEQUAL);
	// Disable depth writing for performance
	glDepthMask(GL_FALSE);
	skyboxShader.Start();
	skyboxShader.LoadViewMatrix(camera);
	skyboxShader.LoadFogColor(GL_CLEAR_COLOR);
	skyboxRenderer.Render(skybox);
	skyboxShader.Stop();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void MasterRenderer::ProcessEntity(const Entity& entity)
{
	auto iter = entities.find(entity.model);
	if (iter != entities.end())
	{
		iter->second.push_back(entity);
	}
	else
	{
		entities[entity.model] = { entity };
	}
}

void MasterRenderer::ProcessTerrain(const Terrain& terrain)
{
	terrains.push_back(terrain);
}

void MasterRenderer::ProcessSkybox(const Skybox& skybox)
{
	this->skybox = skybox;
}