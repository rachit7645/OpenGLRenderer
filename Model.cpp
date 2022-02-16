#include "Model.h"

using namespace Renderer;

Model Renderer::CreateModel(std::vector<f32>& vertices, std::vector<u32>& indices, std::vector<f32>& textureCoords, const std::string& texturePath)
{
	Model model;
	model.vertices = vertices;
	model.indices = indices;

	glGenVertexArrays(1, &model.vaoID);
	glBindVertexArray(model.vaoID);
	model.vboIDs.push_back(CreateVBO(0, 3, vertices));
	model.vboIDs.push_back(CreateVBO(1, 2, textureCoords));
	CreateEBO(&model.eboID, indices);
	glBindVertexArray(0);

	model.texture = Renderer::LoadTexture(texturePath);

	return model;
}

u32 CreateVBO(u32 slot, u32 coordSize, std::vector<f32>& data)
{
	u32 m_vertexBufferID;
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(f32), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(slot, coordSize, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m_vertexBufferID;
}

void CreateEBO(u32* id, std::vector<u32>& data)
{
	glGenBuffers(1, id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(u32), data.data(), GL_STATIC_DRAW);

}