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
#include "Entity.h"

namespace Terrains
{
	constexpr f32 SIZE = 500.0f;
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

		f32 GetHeight(const glm::vec2& worldPos) const;
	private:
		f32 CalculateHeight(int x, int z, Util::Image2D& hMap);
		glm::vec3 CalculateNormal(int x, int z, Util::Image2D& hMap);
	};

	const Terrain* GetCurrent(const std::vector<Terrain>& terrains, const glm::vec2& position);
	const Terrain* GetCurrent(const std::vector<Terrain>& terrains, const Entities::Entity& entity);
}

#endif // TERRAIN_H