#ifndef TERRAIN_H
#define TERRAIN_H

#include <memory>
#include <vector>
#include <string>

#include "GLM.h"
#include "Util.h"
#include "VertexArray.h"
#include "Material.h"
#include "TerrainTextures.h"
#include "Image.h"

namespace Terrains
{
	constexpr auto SIZE = 500;
	constexpr f32 MAX_HEIGHT = 37.0f;
	constexpr u32 MAX_PIXEL_COLOR = 0 << 24 | 255 << 16 | 255 << 8 | 255;

	template<typename T>
	using Array2D = std::vector<std::vector<T>>;

	class Terrain
	{
	public:
		Terrain(const std::string& hMapPath, const glm::vec2& position, const TerrainTextures& textures,
			const Renderer::Material& material = Renderer::Material());

		glm::vec2 position;
		glm::vec2 gridPosition;
		Array2D<f32> heights;

		TerrainTextures textures;
		Renderer::Material material;
		std::shared_ptr<Renderer::VertexArray> vao;
		s32 vertexCount;

		f32 GetHeight(glm::vec2 worldPos) const;
	private:
		f32 CalculateHeight(Util::Image2D& hMap, int x, int z);
		glm::vec3 CalculateNormal(Util::Image2D& hMap, int x, int z);
	};

	const Terrain* GetCurrent(std::vector<Terrain> terrains, glm::vec2 position);
}

#endif // TERRAIN_H