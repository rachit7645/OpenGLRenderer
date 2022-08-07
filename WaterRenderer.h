#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include <memory>
#include <vector>

#include "WaterShader.h"
#include "VertexArray.h"
#include "WaterTile.h"

namespace Renderer
{
	class WaterRenderer
	{
	public:
		using VAO = std::shared_ptr<VertexArray>;
		// Main constructor
		explicit WaterRenderer(Shader::WaterShader& shader);
		Shader::WaterShader& shader;

		void Render(const std::vector<Waters::WaterTile>& waters);
	private:
		VAO vao;
	};
}

#endif
