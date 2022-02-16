#include "Model.h"

using namespace Renderer;

Model Renderer::CreateModel(std::vector<f32> vertices, std::vector<u32> indices)
{
	Model model;
	model.vertices = vertices;
	model.indices = indices;

	glGenVertexArrays(1, &model.vaoID);
	glBindVertexArray(model.vaoID);

	u32 m_vertexBufferID;
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(f32), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	model.vboIDs.push_back(m_vertexBufferID);

	glGenBuffers(1, &model.eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	return model;
}