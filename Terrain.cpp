#include "Terrain.h"

using namespace Terrains;

using Renderer::VertexArray;
using Renderer::Texture;

Terrain::Terrain(const glm::vec2& gridPosition, std::shared_ptr<Renderer::Texture> texture)
	: gridPosition{ gridPosition.x * SIZE, gridPosition.y * SIZE }, texture{ texture }
{
	std::vector<f32> vertices(COUNT * 3);
	std::vector<f32> normals(COUNT * 3);
	std::vector<f32> txCoords(COUNT * 2);
	std::vector<u32> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	size_t vertexPointer = 0;
	for (size_t i = 0; i < VERTEX_COUNT; i++)
	{
		for (size_t j = 0; j < VERTEX_COUNT; j++)
		{
			vertices[vertexPointer * 3] = static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = 0;
			vertices[vertexPointer * 3 + 2] = static_cast<f32>(i) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;
			normals[vertexPointer * 3] = 0;
			normals[vertexPointer * 3 + 1] = 1;
			normals[vertexPointer * 3 + 2] = 0;
			txCoords[vertexPointer * 2] = static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1);
			txCoords[vertexPointer * 2 + 1] = static_cast<f32>(i) / static_cast<f32>(VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}

	vertexPointer = 0;
	for (size_t i = 0; i < VERTEX_COUNT - 1; ++i)
	{
		for (size_t j = 0; j < VERTEX_COUNT - 1; ++j)
		{
			auto topLeft = (i * VERTEX_COUNT) + j;
			auto topRight = topLeft + 1;
			auto bottomLeft = ((i + 1) * VERTEX_COUNT) + j;
			auto bottomRight = bottomLeft + 1;

			indices[vertexPointer++] = topLeft;
			indices[vertexPointer++] = bottomLeft;
			indices[vertexPointer++] = topRight;
			indices[vertexPointer++] = topRight;
			indices[vertexPointer++] = bottomLeft;
			indices[vertexPointer++] = bottomRight;
		}
	}

	vao = std::make_shared<VertexArray>(vertices, indices, txCoords, normals);
	vertexCount = indices.size();
}