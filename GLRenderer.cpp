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

// Draw entities
void GLRenderer::Render(Entity& entity, StaticShader& shader)
{
	glBindVertexArray(entity.model.vao.id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glm::mat4 trans_matrix = Maths::CreateTransformationMatrix(entity.position, entity.rotation, entity.scale);
	shader.LoadTransformationMatrix(trans_matrix);
	shader.LoadShineVariables(entity.model.shineDamper, entity.model.reflectivity);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, entity.model.texture.id);
	glDrawElements(GL_TRIANGLES, entity.model.indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}