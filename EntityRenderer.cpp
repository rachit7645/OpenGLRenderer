#include "EntityRenderer.h"

using namespace Renderer;
using Entities::Entity;
using Shader::StaticShader;

EntityRenderer::EntityRenderer(StaticShader& shaderRef) : shader { shaderRef } {}

void EntityRenderer::Render(const std::unordered_map<std::shared_ptr<Model>, std::vector<Entity>>& entities)
{
	for (const auto& [model, batch] : entities)
	{
		PrepareModel(model);
		for (const auto& entity : batch)
		{
			PrepareInstance(entity);
			glDrawElements(GL_TRIANGLES, model->vertexCount, GL_UNSIGNED_INT, static_cast<const void*>(0));
		}
		UnbindModel(model->material);
	}
}

void EntityRenderer::PrepareModel(const std::shared_ptr<Model>& model)
{
	glBindVertexArray(model->vao->id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->texture->id);
	shader.LoadMaterials(model->material);
	if (model->material.isTransparent)
		DisableCulling();
}

void EntityRenderer::PrepareInstance(const Entity& entity)
{
	glm::mat4 transformation = Maths::CreateTransformationMatrix(entity.position, entity.rotation, entity.scale);
	shader.LoadTransformationMatrix(transformation);
}

void EntityRenderer::UnbindModel(const Material& modelMaterial) 
{
	if (modelMaterial.isTransparent)
		EnableCulling();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::EnableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void EntityRenderer::DisableCulling()
{
	glDisable(GL_CULL_FACE);
}