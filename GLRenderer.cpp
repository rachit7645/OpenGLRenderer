#include "GLRenderer.h"

using namespace Renderer;
using namespace Entities;
using namespace Shader;

// Clear framebuffer for renderering
void GLRenderer::Prepare()
{
	glClearColor(RED, GREEN, BLUE, ALPHA);
	glClear(GL_CLEAR_FLAGS);
}

// Draw Models
void GLRenderer::Render(Entity& entity, StaticShader& shader)
{
	Model& model = entity.model;
	glBindVertexArray(model.vao.id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glm::mat4 trans_matrix = Maths::CreateTransformationMatrix(entity.position, entity.rotation, entity.scale);
	shader.LoadTransformationMatrix(trans_matrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}