#include "Terrain.h"
#include "Maths.h"

using namespace Terrains;

using Renderer::VertexArray;
using Renderer::Texture;
using Renderer::Material;
using Util::Image2D;
using Entities::Entity;

Terrain::Terrain(const std::string& hMapPath, const glm::vec2& position, const TerrainTextures& textures, const Material& material)
	: position(position.x* SIZE, position.y* SIZE), gridPosition(position), textures(textures), material(material)
{
	Image2D hMap(hMapPath);

	auto VERTEX_COUNT = hMap.height;
	auto COUNT = VERTEX_COUNT * VERTEX_COUNT;

	std::vector<f32> vertices(COUNT * 3);
	std::vector<f32> normals(COUNT * 3);
	std::vector<f32> txCoords(COUNT * 2);
	std::vector<u32> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
	heights = Array2D<f32>(VERTEX_COUNT);
	for (auto& row : heights)
	{
		row = std::vector<f32>(VERTEX_COUNT);
	}

	size_t vertexPointer = 0;
	for (ssize_t i = 0; i < VERTEX_COUNT; ++i)
	{
		for (ssize_t j = 0; j < VERTEX_COUNT; ++j)
		{
			vertices[vertexPointer * 3] = static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;
			f32 height = CalculateHeight(static_cast<int>(j), static_cast<int>(i), hMap);
			heights[j][i] = height;
			vertices[vertexPointer * 3 + 1] = height;
			vertices[vertexPointer * 3 + 2] = static_cast<f32>(i) / static_cast<f32>(VERTEX_COUNT - 1) * SIZE;

			glm::vec3 normal = CalculateNormal(j, i, hMap);
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
}

f32 Terrain::CalculateHeight(int x, int z, Image2D& hMap)
{
	f32 height = hMap.GetARGB(x, z);
	height /= MAX_PIXEL_COLOR;
	height = height * 2.0f - 1.0f;
	height *= MAX_HEIGHT;
	return height;
}

glm::vec3 Terrain::CalculateNormal(int x, int z, Image2D& hMap)
{
	auto size = hMap.height;
	f32 heightL = CalculateHeight((x - 1 + size) % size, z, hMap);
	f32 heightR = CalculateHeight((x + 1 + size) % size, z, hMap);
	f32 heightD = CalculateHeight(x, (z - 1 + size) % size, hMap);
	f32 heightU = CalculateHeight(x, (z + 1 + size) % size, hMap);

	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	glm::normalize(normal);
	return normal;
}

f32 Terrain::GetHeight(const glm::vec2& worldPos) const
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

f32 Terrain::GetHeight(const Entity& entity) const
{
	return GetHeight(glm::vec2(entity.position.x, entity.position.z));
}

const Terrain* Terrains::GetCurrent(const std::vector<Terrain>& terrains, const glm::vec2& position)
{
	// Find the grid the position lies in
	glm::vec2 gridPos(std::floor(position.x / SIZE), std::floor(position.y / SIZE));
	// Lambda to pass to std::find_if
	auto find_current = [gridPos](const Terrain& terrain)
	{
		return terrain.gridPosition == gridPos;
	};
	// Store result of std::find_if
	auto result = std::find_if(terrains.begin(), terrains.end(), find_current);
	// Return the item or a nullptr if none was found 
	return result != terrains.end() ? result.base() : nullptr;
}

const Terrain* Terrains::GetCurrent(const std::vector<Terrain>& terrains, const Entity& entity)
{
	return GetCurrent(terrains, glm::vec2(entity.position.x, entity.position.z));
}