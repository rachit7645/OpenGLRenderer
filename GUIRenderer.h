#ifndef GUI_RENDERER_H
#define GUI_RENDERER_H

#include <memory>
#include <vector>

#include "VertexArray.h"
#include "GUIShader.h"
#include "GUI.h"

namespace Renderer
{
	class GUIRenderer
	{
	public:
		using VAO = std::shared_ptr<VertexArray>;
		// Constructor
		explicit GUIRenderer(Shader::GUIShader& shader);
		Shader::GUIShader& shader;

		// Render GUIs
		void Render(const std::vector<GUI*>& guis);
	private:
		VAO vao;
	};
}

#endif
