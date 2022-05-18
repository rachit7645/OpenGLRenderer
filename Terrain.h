#ifndef TERRAIN_H
#define TERRAIN_H

#include <memory>
#include <vector>

#include "GLM.h"
#include "Util.h"
#include "VertexArray.h"
#include "Material.h"
#include "TerrainTextures.h"

namespace Terrains
{
	constexpr auto SIZE = 300, VERTEX_COUNT = 16;
	constexpr auto COUNT = VERTEX_COUNT * VERTEX_COUNT;

	class Terrain
	{
	public:
		Terrain(const glm::vec2& gridPosition, const TerrainTextures& textures, const Renderer::Material& material = Renderer::Material());

		glm::vec2 position;
		TerrainTextures textures;
		Renderer::Material material;
		std::shared_ptr<Renderer::VertexArray> vao;
		s32 vertexCount;
	};
}

#endif // TERRAIN_H