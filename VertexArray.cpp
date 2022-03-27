#include "VertexArray.h"

using namespace Renderer;

VertexArray::VertexArray(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords)
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	buffers["vertices"] = VertexBuffer(0, 3, vert);
	buffers["textureCoords"] = VertexBuffer(1, 2, txCoords);
	buffers["indices"] = VertexBuffer(indi);
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