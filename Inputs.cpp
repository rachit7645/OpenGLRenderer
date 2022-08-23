#include "Inputs.h"

#include "Util.h"

glm::ivec2 mousePos;
glm::ivec2 mouseScroll;
const u8*  keys;

void Inputs::Init()
{
	keys = SDL_GetKeyboardState(nullptr);
}

glm::ivec2& Inputs::MousePos()
{
	return mousePos;
}

glm::ivec2& Inputs::MouseScroll()
{
	return mouseScroll;
}

bool Inputs::IsKeyPressed(SDL_Scancode key)
{
	return static_cast<bool>(keys[key]);
}