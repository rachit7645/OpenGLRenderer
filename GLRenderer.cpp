#include "GLRenderer.h"

using namespace Renderer;
using namespace Entities;
using namespace Shader;

GLRenderer::GLRenderer(StaticShader &shader)
{
	projectionMatrix = CreateProjectionMatrix();
	shader.Start();
	shader.LoadProjectionMatrix(projectionMatrix);
	shader.Stop();
}

glm::mat4 GLRenderer::CreateProjectionMatrix()
{
	return glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
}

// Clear framebuffer for renderering
void GLRenderer::Prepare()
{
	glClearColor(RED, GREEN, BLUE, ALPHA);
	glClear(GL_CLEAR_FLAGS);
}

// Draw Models
void GLRenderer::Render(const Entity& entity, StaticShader& shader)
{
	const Model& model = entity.model;
	glBindVertexArray(model.vao.id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glm::mat4 trans_matrix = Maths::CreateTransformationMatrix(entity.position, entity.rotation, entity.scale);
	shader.LoadTransformationMatrix(trans_matrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}