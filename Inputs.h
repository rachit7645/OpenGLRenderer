#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>

#include "GLM.h"

namespace Inputs
{
	void Init();

	glm::ivec2& MousePos();
	glm::ivec2& MouseScroll();

	bool IsKeyPressed(SDL_Scancode key);
}

#endif