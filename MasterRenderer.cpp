#include "MasterRenderer.h"

using namespace Renderer;

using Entities::Entity;
using Entities::Light;
using Entities::Camera;
using Entities::Skybox;
using Terrains::Terrain;

MasterRenderer::MasterRenderer() :
	renderer(shader), terrainRenderer(terrainShader), skyboxRenderer(skyboxShader),
	matrices(std::make_shared<MatrixBuffer>()), lights(std::make_shared<LightsBuffer>())
{
	matrices->LoadProjection(glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE));
}

void MasterRenderer::Prepare()
{
	glClearColor(GL_CLEAR_COLOR.r, GL_CLEAR_COLOR.g, GL_CLEAR_COLOR.b, GL_CLEAR_COLOR.a);
	glClear(GL_CLEAR_FLAGS);
}

void MasterRenderer::Update(const Light& light, const Camera& camera)
{
	matrices->LoadView(Maths::CreateViewMatrix(camera));
	lights->LoadLight(light);
}

void MasterRenderer::Render(const Light& light, const Camera& camera)
{
	Prepare();
	Update(light, camera);

	RenderEntities();
	RenderTerrains();
	RenderSkybox();

	entities.clear();
	terrains.clear();
}

void MasterRenderer::RenderEntities()
{
	shader.Start();
	shader.LoadSkyColour(GL_CLEAR_COLOR);
	renderer.Render(entities);
	shader.Stop();
}

void MasterRenderer::RenderTerrains()
{
	terrainShader.Start();
	terrainShader.LoadSkyColour(GL_CLEAR_COLOR);
	terrainRenderer.Render(terrains);
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