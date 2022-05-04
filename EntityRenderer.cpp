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
		UnbindModel();
	}
}

void EntityRenderer::PrepareModel(const std::shared_ptr<Model>& model)
{
	glBindVertexArray(model->vao->id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shader.LoadShineVariables(model->shineDamper, model->reflectivity);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->texture->id);
}

void EntityRenderer::PrepareInstance(const Entity& entity)
{
	glm::mat4 trans_matrix = Maths::CreateTransformationMatrix(entity.position, entity.rotation, entity.scale);
	shader.LoadTransformationMatrix(trans_matrix);
}

void EntityRenderer::UnbindModel() 
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}