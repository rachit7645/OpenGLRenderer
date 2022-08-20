#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include <memory>
#include <vector>

#include "WaterShader.h"
#include "VertexArray.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

namespace Renderer
{
	class WaterRenderer
	{
	public:
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
	};
}

#endif
