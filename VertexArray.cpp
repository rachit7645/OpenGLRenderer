#include "VertexArray.h"

using namespace Renderer;

VertexArray::VertexArray(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords, std::vector<f32> &norms)
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	buffers["vertices"] = std::make_shared<VertexBuffer>(0, 3, vert);
	buffers["textureCoords"] = std::make_shared<VertexBuffer>(1, 2, txCoords);
	buffers["normals"] = std::make_shared<VertexBuffer>(2, 3, norms);
	buffers["indices"] = std::make_shared<VertexBuffer>(indi);
	glBindVertexArray(0);
}

VertexArray::VertexArray()
{
	id = 0;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}