#include "GLRenderer.h"

// Clear framebuffer for renderering
void Renderer::Prepare()
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(Model model)
{
	glBindVertexArray(model.vaoID);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}