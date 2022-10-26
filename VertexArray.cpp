#include "VertexArray.h"

using namespace Renderer;

VertexArray::VertexArray
(
	const std::vector<f32>& vertices,
	const std::vector<u32>& indices,
	const std::vector<f32>& txCoords,
	const std::vector<f32>& normals
)
	: vertexCount(static_cast<GLsizei>(indices.size()))
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	buffers["vertices"] = std::make_shared<VertexBuffer>();
	buffers["vertices"]->CreateBuffer();
	buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
	buffers["vertices"]->BufferData(GL_ARRAY_BUFFER, vertices);
	buffers["vertices"]->SetVertexAttribute(0, 3, GL_FLOAT, 0, nullptr);
	buffers["vertices"]->Unbind(GL_ARRAY_BUFFER);

	buffers["txCoords"] = std::make_shared<VertexBuffer>();
	buffers["txCoords"]->CreateBuffer();
	buffers["txCoords"]->Bind(GL_ARRAY_BUFFER);
	buffers["txCoords"]->BufferData(GL_ARRAY_BUFFER, txCoords);
	buffers["txCoords"]->SetVertexAttribute(1, 2, GL_FLOAT, 0, nullptr);
	buffers["txCoords"]->Unbind(GL_ARRAY_BUFFER);

	buffers["normals"] = std::make_shared<VertexBuffer>();
	buffers["normals"]->CreateBuffer();
	buffers["normals"]->Bind(GL_ARRAY_BUFFER);
	buffers["normals"]->BufferData(GL_ARRAY_BUFFER, normals);
	buffers["normals"]->SetVertexAttribute(2, 3, GL_FLOAT, 0, nullptr);
	buffers["normals"]->Unbind(GL_ARRAY_BUFFER);

	buffers["indices"] = std::make_shared<VertexBuffer>();
	buffers["indices"]->CreateBuffer();
	buffers["indices"]->Bind(GL_ELEMENT_ARRAY_BUFFER);
	buffers["indices"]->BufferData(GL_ELEMENT_ARRAY_BUFFER, indices);

	glBindVertexArray(0);
}

VertexArray::VertexArray
(
	GLint coordSize,
	const std::vector<f32>& vertices
)
	: vertexCount(static_cast<GLsizei>(vertices.size() / coordSize))
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glEnableVertexAttribArray(0);

	buffers["vertices"] = std::make_shared<VertexBuffer>();
	buffers["vertices"]->CreateBuffer();
	buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
	buffers["vertices"]->BufferData(GL_ARRAY_BUFFER, vertices);
	buffers["vertices"]->SetVertexAttribute(0, coordSize, GL_FLOAT, 0, nullptr);
	buffers["vertices"]->Unbind(GL_ARRAY_BUFFER);

	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	// ID must not be zero
	if (id)
	{
		glDeleteVertexArrays(1, &id);
	}
}