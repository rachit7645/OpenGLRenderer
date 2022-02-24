#include "VertexArray.h"

using namespace Renderer;

VertexArray::VertexArray(std::vector<f32>& m_vertices, std::vector<u32>& m_indices, std::vector<f32>& m_textureCoords)
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	buffers["vertices"] = VertexBuffer(0, 3, m_vertices);
	buffers["textureCoords"] = VertexBuffer(1, 2, m_textureCoords);
	buffers["indices"] = VertexBuffer(m_indices);
	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	Delete();
}

VertexArray::VertexArray(VertexArray &&other) : id(other.id)
{
	other.id = 0;
}

void VertexArray::Delete()
{
	if (id)
		glDeleteVertexArrays(1, &id);
}