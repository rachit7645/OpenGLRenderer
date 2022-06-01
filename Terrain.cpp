#include "Terrain.h"
#include "Maths.h"

using namespace Terrains;

using Renderer::VertexArray;
using Renderer::Texture;
using Renderer::Material;
using Util::Image2D;

Terrain::Terrain(const std::string &hMapPath, const glm::vec2 &position, const TerrainTextures &textures, const Material &material)
	: position(position.x *SIZE, position.y *SIZE), gridPosition(position), textures(textures), material(material)
{
	Image2D hMap(hMapPath);

	auto VERTEX_COUNT = hMap.height;
	auto COUNT = VERTEX_COUNT * VERTEX_COUNT;

	std::vector<f32> vertices(COUNT * 3);
	std::vector<f32> normals(COUNT * 3);
	std::vector<f32> txCoords(COUNT * 2);
	std::vector<u32> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
	heights = Array2D<f32>(VERTEX_COUNT);
	for (auto &row : heights)
	{
		row = std::vector<f32>(VERTEX_COUNT);
	}

	size_t vertexPointer = 0;
	for (ssize_t i = 0; i < VERTEX_COUNT; ++i)
	{
		for (ssize_t j = 0; j < VERTEX_COUNT; ++j)
		{
			vertices[vertexPointer * 3] = static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;
			f32 height = CalculateHeight(hMap, static_cast<int>(j), static_cast<int>(i));
			heights[j][i] = height;
			vertices[vertexPointer * 3 + 1] = height;
			vertices[vertexPointer * 3 + 2] = static_cast<f32>(i) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;

			glm::vec3 normal = CalculateNormal(hMap, j, i);
			normals[vertexPointer * 3] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;

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

f32 Terrain::CalculateHeight(Image2D &hMap, int x, int z)
{
	f32 height = hMap.GetRGB(x, z);
	height /= MAX_PIXEL_COLOR;
	height = height * 2.0f - 1.0f;
	height *= MAX_HEIGHT;
	return height;
}

glm::vec3 Terrain::CalculateNormal(Image2D &hMap, int x, int z)
{
	auto size = hMap.height;
	f32 heightL = CalculateHeight(hMap, (x - 1 + size) % size, z);
	f32 heightR = CalculateHeight(hMap, (x + 1 + size) % size, z);
	f32 heightD = CalculateHeight(hMap, x, (z - 1 + size) % size);
	f32 heightU = CalculateHeight(hMap, x, (z + 1 + size) % size);

	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	glm::normalize(normal);
	return normal;
}

f32 Terrain::GetHeight(glm::vec2 worldPos) const
{
	glm::vec2 terrainPos = worldPos - position;
	f32 gridSize = SIZE / static_cast<f32>(heights.size() - 1);
	glm::vec2 gridPos = glm::floor(terrainPos / gridSize);

	if (gridPos.x >= heights.size() - 1 || gridPos.y >= heights.size() - 1 || gridPos.x < 0 || gridPos.y < 0)
	{
		return 0.0f;
	}

	glm::vec2 coords(std::fmod(terrainPos.x, gridSize) / gridSize, std::fmod(terrainPos.y, gridSize) / gridSize);
	f32 height;

	if (coords.x <= (1 - coords.y))
	{
		height = Maths::BarryCentric(
			glm::vec3(0, heights[gridPos.x][gridPos.y], 0),
			glm::vec3(1, heights[gridPos.x + 1][gridPos.y], 0),
			glm::vec3(0, heights[gridPos.x][gridPos.y + 1], 1),
			coords
		);
	}
	else
	{
		height = Maths::BarryCentric(
			glm::vec3(1, heights[gridPos.x + 1][gridPos.y], 0),
			glm::vec3(1, heights[gridPos.x + 1][gridPos.y + 1], 1),
			glm::vec3(0, heights[gridPos.x][gridPos.y + 1], 1),
			coords
		);
	}

	return height;
}

const Terrain *Terrains::GetCurrent(std::vector<Terrain> terrains, glm::vec2 position)
{
	glm::vec2 gridPos(glm::floor(position.x / SIZE), glm::floor(position.y / SIZE));

	for (const auto& terrain : terrains)
	{
		if (terrain.gridPosition == gridPos)
		{
			return &terrain;
		}
	}
	
	return nullptr;
}