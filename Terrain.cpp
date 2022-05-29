#include "Terrain.h"

using namespace Terrains;

using Renderer::VertexArray;
using Renderer::Texture;
using Renderer::Material;
using Util::Image2D;

Terrain::Terrain(const std::string& hMapPath, const glm::vec2 &position, const TerrainTextures &textures, const Material &material)
	: position{ position.x * SIZE, position.y * SIZE }, textures{ textures }, material{ material }
{
	Image2D hMap(hMapPath);

	auto VERTEX_COUNT = hMap.height;
	auto COUNT = VERTEX_COUNT * VERTEX_COUNT;

	std::vector<f32> vertices(COUNT * 3);
	std::vector<f32> normals(COUNT * 3);
	std::vector<f32> txCoords(COUNT * 2);
	std::vector<u32> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

	size_t vertexPointer = 0;
	for (ssize_t i = 0; i < VERTEX_COUNT; ++i)
	{
		for (ssize_t j = 0; j < VERTEX_COUNT; ++j)
		{
			vertices[vertexPointer * 3] = static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer * 3 + 1] = GetHeight(hMap, static_cast<int>(j), static_cast<int>(i));
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
	for (ssize_t i = 0; i < VERTEX_COUNT - 1; ++i)
	{
		for (ssize_t j = 0; j < VERTEX_COUNT - 1; ++j)
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
	vertexCount = static_cast<s32>(indices.size());
}

f32 Terrain::GetHeight(Image2D& hMap, int x, int y)
{
	f32 height = hMap.GetRGB(x, y);
	height /= MAX_PIXEL_COLOR;
	height = height * 2.0f - 1.0f;
	height *= MAX_HEIGHT;
	return height;
}