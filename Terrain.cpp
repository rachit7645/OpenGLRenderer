#include "Terrain.h"

using namespace Terrains;

using Renderer::VertexArray;
using Renderer::Texture;

Terrain::Terrain(const glm::vec2 &grid_pos, std::shared_ptr<VertexArray> vao,
	std::shared_ptr<Texture> texture, size_t vertex_count)
	: grid_position{ grid_pos.x * SIZE, grid_pos.y * SIZE },
	vao{ vao }, texture{ texture }, vertex_count{ vertex_count } {}

Terrain Terrains::GenerateTerrain(const glm::vec2 &grid_pos, std::shared_ptr<Texture> texture)
{
	constexpr auto COUNT = VERTEX_COUNT * VERTEX_COUNT;
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

	size_t pointer = 0;
	for (size_t gz = 0; gz < VERTEX_COUNT - 1; ++gz)
	{
		for (size_t gx = 0; gx < VERTEX_COUNT - 1; ++gx)
		{
			auto topLeft = (gz * VERTEX_COUNT) + gx;
			auto topRight = topLeft + 1;
			auto bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			auto bottomRight = bottomLeft + 1;

			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	return { grid_pos, std::make_shared<VertexArray>(vertices, indices, txCoords, normals), texture, indices.size()};
}