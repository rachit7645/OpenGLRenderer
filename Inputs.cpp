#include "Inputs.h"

#include "Util.h"

glm::ivec2 m_mousePos;
glm::ivec2 m_mouseScroll;
const u8*  m_keys;

void Inputs::Init()
{
	m_keys = SDL_GetKeyboardState(nullptr);
}

glm::ivec2& Inputs::GetMousePos()
{
	return m_mousePos;
}

glm::ivec2& Inputs::GetMouseScroll()
{
	return m_mouseScroll;
}

bool Inputs::IsKeyPressed(SDL_Scancode key)
{
	return static_cast<bool>(m_keys[key]);
}