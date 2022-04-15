#include "VertexBuffer.h"

using namespace Renderer;

VertexBuffer::VertexBuffer(u32 slot, u32 coordSize, std::vector<f32> &data)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(f32), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(slot, coordSize, GL_FLOAT, GL_FALSE, 0, static_cast<const void*>(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::VertexBuffer(std::vector<u32> &data)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(u32), data.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer()
{
	id = 0;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}