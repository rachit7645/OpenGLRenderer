#include "EntityRenderer.h"

using namespace Renderer;
using Entities::Entity;
using Shader::StaticShader;

EntityRenderer::EntityRenderer(StaticShader &shaderRef) : shader{ shaderRef } {}

void EntityRenderer::Render(const std::unordered_map<std::shared_ptr<Model>, std::vector<Entity>> &entities)
{
	for (const auto &[model, batch] : entities)
	{
		PrepareModel(model);
		for (const auto &mesh : model->meshes)
		{
			PrepareMesh(mesh);
			for (const auto &entity : batch)
			{
				PrepareInstance(entity);
				glDrawElements(GL_TRIANGLES, mesh.vertexCount, GL_UNSIGNED_INT, static_cast<const void *>(0));
			}
			UnbindMesh();
		}
		UnbindModel(model);
	}
}

void EntityRenderer::PrepareModel(const std::shared_ptr<Model> &model)
{
	shader.LoadMaterials(model->material);
	if (model->material.isTransparent)
		DisableCulling();
}

void EntityRenderer::PrepareMesh(const Mesh &mesh)
{
	glBindVertexArray(mesh.vao->id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture->id);
}

void EntityRenderer::PrepareInstance(const Entity &entity)
{
	glm::mat4 transformation = Maths::CreateTransformationMatrix(entity.position, entity.rotation, entity.scale);
	shader.LoadTransformationMatrix(transformation);
}

void EntityRenderer::UnbindMesh()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::UnbindModel(const std::shared_ptr<Model> &model)
{
	if (model->material.isTransparent)
		EnableCulling();
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