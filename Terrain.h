#ifndef TERRAIN_H
#define TERRAIN_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Util.h"
#include "Model.h"
#include "VertexArray.h"

namespace Terrains
{
	constexpr auto SIZE = 300, VERTEX_COUNT = 64;
	constexpr auto COUNT = VERTEX_COUNT * VERTEX_COUNT;

	class Terrain
	{
	public:
		Terrain(const glm::vec2& gridPosition, std::shared_ptr<Renderer::Texture> texture);

		glm::vec2 gridPosition;
		std::shared_ptr<Renderer::VertexArray> vao;
		std::shared_ptr<Renderer::Texture> texture;
		f32 shineDamper = 1.0f;
		f32 reflectivity = 0.0f;
		size_t vertexCount;
	};
}

#endif // TERRAIN_H