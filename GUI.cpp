#include "GUI.h"

using namespace Renderer;

GUI::GUI
(
	TxPtr texture,
	const glm::vec2& position,
	const glm::vec2& scale
)
	: texture(std::move(texture)),
	  position(position),
	  scale(scale)
{
}