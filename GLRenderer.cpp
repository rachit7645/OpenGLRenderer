#include "GLRenderer.h"

// Clear framebuffer for renderering
void Renderer::Prepare()
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Draw Models
void Renderer::Render(Model model)
{
	glBindVertexArray(model.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.texture.textureID);
	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}