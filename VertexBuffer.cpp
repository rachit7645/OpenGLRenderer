#include "VertexBuffer.h"

using namespace Renderer;

std::map<u32, u32> vbo_ref_count;

VertexBuffer::VertexBuffer(u32 slot, u32 coordSize, std::vector<f32> &data)
{
	glGenBuffers(1, &id);
	vbo_ref_count[id] = 1;
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(f32), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(slot, coordSize, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::VertexBuffer(std::vector<u32> &data)
{
	glGenBuffers(1, &id);
	vbo_ref_count[id] = 1;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(u32), data.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer()
{
	id = 0;
	vbo_ref_count[id] = 1;
}

VertexBuffer::~VertexBuffer()
{
	vbo_ref_count[id]--;
	if (vbo_ref_count[id] <= 0)
		glDeleteBuffers(1, &id);
}

VertexBuffer::VertexBuffer(const VertexBuffer &other) : id(other.id)
{
	vbo_ref_count[id]++;
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) : id(other.id)
{
	vbo_ref_count[id]++;
}

void VertexBuffer::IncRefCount()
{
	vbo_ref_count[id]++;
}