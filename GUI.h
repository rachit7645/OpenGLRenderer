#ifndef GUI_H
#define GUI_H

#include <memory>

#include "GLM.h"
#include "Texture.h"

namespace Renderer
{
	class GUI
	{
	public:
		using TxPtr = std::shared_ptr<Texture>;
		// Default constructor
		GUI() = default;
		// Main constructor
		GUI
		(
			TxPtr texture,
			const glm::vec2& position,
			const glm::vec2& scale
		);

		TxPtr     texture  = nullptr;
		glm::vec2 position = {0.0f, 0.0f};
		glm::vec2 scale    = {1.0f, 1.0f};
	};
}

#endif
