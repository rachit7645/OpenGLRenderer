#include "MasterRenderer.h"

using namespace Renderer;
using namespace Entities;

MasterRenderer::MasterRenderer() : shader{ VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH }, renderer{ shader },
	terrainShader{ TERRAIN_VERTEX_SHADER_PATH, TERRAIN_FRAGMENT_SHADER_PATH }, terrainRenderer{ terrainShader }
{
	glm::mat4 projection = glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
	shader.Start();
	shader.LoadProjectionMatrix(projection);
	shader.Stop();
	terrainShader.Start();
	terrainShader.LoadProjectionMatrix(projection);
	terrainShader.Stop();
}

void MasterRenderer::Prepare()
{
	glClearColor(RED, GREEN, BLUE, ALPHA);
	glClear(GL_CLEAR_FLAGS);
}

void MasterRenderer::Render(Light &light, Camera &camera)
{
	Prepare();
	shader.Start();
	shader.LoadLight(light);
	shader.LoadViewMatrix(camera);
	renderer.Render(entities);
	shader.Stop();
	terrainShader.Start();
	terrainShader.LoadLight(light);
	terrainShader.LoadViewMatrix(camera);
	terrainRenderer.Render(terrains);
	terrainShader.Stop();
	entities.clear();
	terrains.clear();
}

void MasterRenderer::ProcessEntity(Entities::Entity &entity)
{
	auto iter = entities.find(entity.model);
	if (iter != entities.end())
	{
		iter->second.push_back(entity);
	}
	else
	{
		std::vector<Entity> newBatch;
		newBatch.push_back(entity);
		entities[entity.model] = newBatch;
	}
}

void MasterRenderer::ProcessTerrain(Terrains::Terrain &terrain)
{
	terrains.push_back(terrain);
}