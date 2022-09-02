#include "Terrain.h"

#include <memory>
#include <algorithm>

#include "Maths.h"

using namespace Terrains;

using Renderer::VertexArray;
using Renderer::Texture;
using Renderer::Material;
using Util::Image2D;
using Entities::Entity;

Terrain::Terrain
(
	const std::string& hMapPath,
	const glm::vec2& position,
	const TerrainTextures& textures
)
	: position(position.x * TERRAIN_SIZE, position.y * TERRAIN_SIZE),
	  gridPosition(position),
	  textures(textures)
{
	auto hMap = std::make_shared<Image2D>(hMapPath);

	auto VERTEX_COUNT = hMap->height;
	auto COUNT        = VERTEX_COUNT * VERTEX_COUNT;

	std::vector<f32> vertices(COUNT * 3);
	std::vector<f32> normals(COUNT * 3);
	std::vector<f32> txCoords(COUNT * 2);
	std::vector<u32> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));
	Util::InitArray2D<f32>(heights, VERTEX_COUNT);

	usize vertexPointer = 0;
	for (ssize i = 0; i < VERTEX_COUNT; ++i)
	{
		for (ssize j = 0; j < VERTEX_COUNT; ++j)
		{
			f32 height = CalculateHeight(static_cast<int>(j), static_cast<int>(i), hMap);
			heights[j][i] = height;

			vertices[vertexPointer * 3] 	= static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1) * TERRAIN_SIZE;
			vertices[vertexPointer * 3 + 1] = height;
			vertices[vertexPointer * 3 + 2] = static_cast<f32>(i) / static_cast<f32>(VERTEX_COUNT - 1) * TERRAIN_SIZE;

			glm::vec3 normal = CalculateNormal(static_cast<int>(j), static_cast<int>(i), hMap);
			normals[vertexPointer * 3]     = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;

			txCoords[vertexPointer * 2] 	= static_cast<f32>(j) / static_cast<f32>(VERTEX_COUNT - 1);
			txCoords[vertexPointer * 2 + 1] = static_cast<f32>(i) / static_cast<f32>(VERTEX_COUNT - 1);

			vertexPointer++;
		}
	}

	vertexPointer = 0;
	for (ssize i = 0; i < VERTEX_COUNT - 1; ++i)
	{
		for (ssize j = 0; j < VERTEX_COUNT - 1; ++j)
		{
			auto topLeft	 = (i * VERTEX_COUNT) + j;
			auto topRight	 = topLeft + 1;
			auto bottomLeft  = ((i + 1) * VERTEX_COUNT) + j;
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

f32 Terrain::CalculateHeight(int x, int z, const ImgPtr& hMap)
{
	f32 height = hMap->GetRed(x, z);
	height -= IMAGE_MAX_PIXEL_COLOR / 2.0f;
	height /= IMAGE_MAX_PIXEL_COLOR / 2.0f;
	height *= TERRAIN_MAX_HEIGHT;
	return height;
}

glm::vec3 Terrain::CalculateNormal(int x, int z, const ImgPtr& hMap)
{
	auto size   = hMap->height;
	f32 heightL = CalculateHeight((x - 1 + size) % size, z, hMap);
	f32 heightR = CalculateHeight((x + 1 + size) % size, z, hMap);
	f32 heightD = CalculateHeight(x, (z - 1 + size) % size, hMap);
	f32 heightU = CalculateHeight(x, (z + 1 + size) % size, hMap);

	return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
}

f32 Terrain::GetHeight(const glm::vec2& worldPos) const
{
	f32 terrainX = worldPos.x - position.x;
	f32 terrainZ = worldPos.y - position.y;

	f32 gridSize  = TERRAIN_SIZE / static_cast<f32>(heights.size() - 1);
	ssize gridX = std::floor(terrainX / gridSize);
	ssize gridZ = std::floor(terrainZ / gridSize);

	if
	(
		gridX >= static_cast<ssize>(heights.size() - 1) ||
		gridZ >= static_cast<ssize>(heights.size() - 1) ||
		gridX < 0 ||
		gridZ < 0
	)
	{
		return 0.0f;
	}

	f32 xCoord = std::fmod(terrainX, gridSize) / gridSize;
	f32 zCoord = std::fmod(terrainZ, gridSize) / gridSize;

	f32 height;
	if (xCoord <= (1 - zCoord))
	{
		height = Maths::BarryCentric(
			glm::vec3(0, heights[gridX][gridZ], 0),
			glm::vec3(1, heights[gridX + 1][gridZ], 0),
			glm::vec3(0, heights[gridX][gridZ + 1], 1),
			glm::vec2(xCoord, zCoord)
		);
	}
	else
	{
		height = Maths::BarryCentric(
			glm::vec3(1, heights[gridX + 1][gridZ], 0),
			glm::vec3(1, heights[gridX + 1][gridZ + 1], 1),
			glm::vec3(0, heights[gridX][gridZ + 1], 1),
			glm::vec2(xCoord, zCoord)
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
	glm::vec2 gridPos(std::floor(position.x / TERRAIN_SIZE), std::floor(position.y / TERRAIN_SIZE));
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