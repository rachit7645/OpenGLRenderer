#include "GUIRenderer.h"

#include "Util.h"
#include "Maths.h"

// TODO: Use instancing to draw these
// TODO: Do I need this?

using namespace Renderer;

const std::vector<f32> QUAD_VERTICES =
{
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f
};

GUIRenderer::GUIRenderer(Shader::GUIShader& shader)
	: shader(shader),
	  m_vao(std::make_shared<VertexArray>(2, QUAD_VERTICES))
{
}

void GUIRenderer::Render(const std::vector<GUI>& guis)
{
	glBindVertexArray(m_vao->id);
	for (const auto& gui : guis)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui.texture->id);
		glm::mat4 matrix = Maths::CreateModelMatrix2D(gui.position, gui.scale);
		shader.LoadModelMatrix(matrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vao->vertexCount);
	}
	glBindVertexArray(0);
}
