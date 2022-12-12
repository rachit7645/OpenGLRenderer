#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"

namespace Inputs
{
	class InputHandler
	{
	private:
		// Main constructor
		InputHandler();
	public:
		// Get mouse position
		glm::ivec2& GetMousePos();
		// Get mouse scroll
		glm::ivec2& GetMouseScroll();
		// Check if key is pressed
		bool IsKeyPressed(SDL_Scancode key);
	private:
		// Mouse Position
		glm::ivec2 m_mousePos = {0, 0};
		// Mouse scroll
		glm::ivec2 m_mouseScroll = {0, 0};
		// Key array
		const u8* m_keys = nullptr;
	public:
		// Get instance
		static InputHandler& GetInstance();
	};
}

#endif