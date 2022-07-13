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
	buffers["vertices"] 	 = std::make_shared<VertexBuffer>(0, 3, vertices);
	buffers["textureCoords"] = std::make_shared<VertexBuffer>(1, 2, txCoords);
	buffers["normals"]		 = std::make_shared<VertexBuffer>(2, 3, normals);
	buffers["indices"]		 = std::make_shared<VertexBuffer>(indices);
	glBindVertexArray(0);
}

VertexArray::VertexArray(GLuint coordSize, const std::vector<f32>& vertices)
	: vertexCount(static_cast<GLsizei>(vertices.size() / coordSize))
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	buffers["vertices"] = std::make_shared<VertexBuffer>(0, coordSize, vertices);
	glBindVertexArray(0);
}

VertexArray::VertexArray()
{
	id = 0;
}

VertexArray::~VertexArray()
{
	// ID must not be zero
	if (id)
	{
		glDeleteVertexArrays(1, &id);
	}
}