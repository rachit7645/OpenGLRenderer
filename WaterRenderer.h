#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include <memory>
#include <vector>

#include "WaterShader.h"
#include "VertexArray.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"
#include "Texture.h"

namespace Renderer
{
	constexpr auto WATER_DUDV_MAP_PATH = "gfx/waterDUDV.png";
	constexpr auto WATER_WAVE_SPEED    = 0.03f;

	class WaterRenderer
	{
	public:
		// Usings
		using VAO   = std::shared_ptr<VertexArray>;
		using TxPtr = std::shared_ptr<Texture>;
		// Main constructor
		explicit WaterRenderer(Shader::WaterShader& shader);
		Shader::WaterShader& shader;

		void Render(const std::vector<Waters::WaterTile>& waters, const Waters::WaterFrameBuffers& waterFBOs);
	private:
		void Prepare(const Waters::WaterFrameBuffers& waterFBOs);
		void PrepareWater(const Waters::WaterTile& water);
		void Unbind();

		VAO   vao;
		TxPtr dudvMap;
		f32   moveFactor;
	};
}

#endif
