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
	constexpr auto WATER_WAVE_SPEED = 0.04f;

	class WaterRenderer
	{
	public:
		// Usings
		using VAO = std::shared_ptr<VertexArray>;
		// Main constructor
		explicit WaterRenderer(Shader::WaterShader& shader);
		Shader::WaterShader& shader;

		void Render(const std::vector<Waters::WaterTile>& waters, const Waters::WaterFrameBuffers& waterFBOs);
	private:
		void Prepare(const Waters::WaterFrameBuffers& waterFBOs);
		void PrepareWater(const Waters::WaterTile& water);
		void Unbind();

		VAO vao;
		f32 moveFactor = 0.0f;
	};
}

#endif
