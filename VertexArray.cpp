#include "VertexArray.h"

using namespace Renderer;

std::map<u32, s64> vao_ref_count;

VertexArray::VertexArray(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords, std::vector<f32> &norms)
{
	glGenVertexArrays(1, &id);
	vao_ref_count[id] = 1;
	glBindVertexArray(id);
	buffers["vertices"] = VertexBuffer(0, 3, vert);
	buffers["textureCoords"] = VertexBuffer(1, 2, txCoords);
	buffers["normals"] = VertexBuffer(2, 3, norms);
	buffers["indices"] = VertexBuffer(indi);
	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	vao_ref_count[id]--;
	if (vao_ref_count[id] <= 0)
		glDeleteVertexArrays(1, &id);
}

VertexArray::VertexArray(const VertexArray &other) : id(other.id)
{
	vao_ref_count[id]++;
}

VertexArray::VertexArray(VertexArray &&other) : id(other.id)
{
	vao_ref_count[id]++;
}

void VertexArray::IncRefCount()
{
	vao_ref_count[id]++;
}