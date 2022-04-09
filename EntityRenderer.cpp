#include "EntityRenderer.h"

using namespace Renderer;
using namespace Entities;
using namespace Shader;

EntityRenderer::EntityRenderer(StaticShader& sh) : shader { sh } 
{
	shader.Start();
	glm::mat4 projection = Maths::CreateProjectionMatrix();
	shader.LoadProjectionMatrix(projection);
	shader.Stop();
}

// Clear framebuffer for renderering
void EntityRenderer::Prepare()
{
	glClearColor(RED, GREEN, BLUE, ALPHA);
	glClear(GL_CLEAR_FLAGS);
}

void EntityRenderer::Render(std::unordered_map<Model*, std::vector<Entities::Entity>> entities)
{
	for (auto& [model, batch] : entities)
	{
		PrepareModel(model);
		for (auto& entity : batch)
		{
			PrepareInstance(entity);
			glDrawElements(GL_TRIANGLES, entity.model.indices.size(), GL_UNSIGNED_INT, 0);
		}
		UnbindModel();
	}
}

void EntityRenderer::PrepareModel(Model* model)
{
	glBindVertexArray(model->vao.id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	shader.LoadShineVariables(model->shineDamper, model->reflectivity);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->texture.id);
}

void EntityRenderer::PrepareInstance(Entity& entity)
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