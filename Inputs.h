#ifndef INPUTS_H
#define INPUTS_H

#include <SDL2/SDL.h>

#include "GLM.h"

namespace Inputs
{
	void Init();

	glm::ivec2& GetMousePos();
	glm::ivec2& GetMouseScroll();

	bool IsKeyPressed(SDL_Scancode key);
}

#endif