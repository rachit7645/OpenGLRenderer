#ifndef TERRAIN_H
#define TERRAIN_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Util.h"
#include "VertexArray.h"
#include "Material.h"
#include "Texture.h"

namespace Terrains
{
	constexpr auto SIZE = 300, VERTEX_COUNT = 16;
	constexpr auto COUNT = VERTEX_COUNT * VERTEX_COUNT;

	class Terrain
	{
	public:
		Terrain(const glm::vec2& gridPosition, const Renderer::Material& material, std::shared_ptr<Renderer::Texture> texture);

		glm::vec2 gridPosition;
		Renderer::Material material;

		std::shared_ptr<Renderer::VertexArray> vao;
		std::shared_ptr<Renderer::Texture> texture;

		s32 vertexCount;
	};
}

#endif // TERRAIN_H