#include "MasterRenderer.h"

using namespace Renderer;
using namespace Entities;

MasterRenderer::MasterRenderer() : shader{ VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH }, renderer{ shader } {}

void MasterRenderer::Render(Light &light, Camera &camera)
{
	renderer.Prepare();
	shader.Start();
	shader.LoadLight(light);
	shader.LoadViewMatrix(camera);
	renderer.Render(entities);
	shader.Stop();
	entities.clear();
}

void MasterRenderer::ProcessEntity(Entities::Entity& entity)
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