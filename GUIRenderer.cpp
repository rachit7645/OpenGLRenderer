#include "GUIRenderer.h"

#include "Util.h"
#include "Maths.h"

using namespace Renderer;

GUIRenderer::GUIRenderer(Shader::GUIShader& shader)
	: shader(shader)
{
	const std::vector<f32> QUAD_VERTICES =
	{
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f
	};

	m_vao = std::make_shared<VertexArray>(2, QUAD_VERTICES);
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
