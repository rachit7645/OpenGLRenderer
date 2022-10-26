#include "VertexArray.h"

using namespace Renderer;

VertexArray::VertexArray(const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
	: vertexCount(static_cast<GLsizei>(indices.size()))
{
	// Create VAO
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	// Enable attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Create vertex buffer
	buffers["vertices"] = std::make_shared<VertexBuffer>();
	buffers["vertices"]->CreateBuffer();
	buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
	buffers["vertices"]->BufferData(GL_ARRAY_BUFFER, vertices);
	// Enable position
	buffers["vertices"]->SetVertexAttribute
	(
		0,
		3,
		GL_FLOAT,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, position))
	);
	// Enable texture coordinates
	buffers["vertices"]->SetVertexAttribute
	(
		1,
		2,
		GL_FLOAT,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, txCoord))
	);
	// Enable normals
	buffers["vertices"]->SetVertexAttribute
	(
		2,
		3,
		GL_FLOAT,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, normal))
	);
	// Unbind
	buffers["vertices"]->Unbind(GL_ARRAY_BUFFER);

	// Create index buffer
	buffers["indices"] = std::make_shared<VertexBuffer>();
	buffers["indices"]->CreateBuffer();
	buffers["indices"]->Bind(GL_ELEMENT_ARRAY_BUFFER);
	buffers["indices"]->BufferData(GL_ELEMENT_ARRAY_BUFFER, indices);

	// Unbind
	glBindVertexArray(0);
}

VertexArray::VertexArray
(
	GLint coordSize,
	const std::vector<f32>& vertices
)
	: vertexCount(static_cast<GLsizei>(vertices.size() / coordSize))
{
	// Create VAO
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	// Enable attributes
	glEnableVertexAttribArray(0);

	// Create vertex buffer
	buffers["vertices"] = std::make_shared<VertexBuffer>();
	buffers["vertices"]->CreateBuffer();
	buffers["vertices"]->Bind(GL_ARRAY_BUFFER);
	buffers["vertices"]->BufferData(GL_ARRAY_BUFFER, vertices);
	buffers["vertices"]->SetVertexAttribute(0, coordSize, GL_FLOAT, 0, nullptr);
	buffers["vertices"]->Unbind(GL_ARRAY_BUFFER);

	// Unbind
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