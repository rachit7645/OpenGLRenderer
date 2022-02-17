#include "Model.h"

using namespace Renderer;

Model::Model(std::vector<f32>& m_vertices, std::vector<u32>& m_indices,
			 std::vector<f32>& m_textureCoords, const std::string& texturePath)
			: texture(texturePath)
{
	vertices = m_vertices;
	indices = m_indices;
	textureCoords = m_textureCoords;

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	vboIDs["vertices"] = CreateVBO(0, 3, vertices);
	vboIDs["textureCoords"] = CreateVBO(1, 2, textureCoords);
	eboID = CreateEBO(indices);
	glBindVertexArray(0);
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

u32 CreateEBO(std::vector<u32>& data)
{
	u32 m_elementBufferID;
	glGenBuffers(1, &m_elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(u32), data.data(), GL_STATIC_DRAW);
	return m_elementBufferID;
}