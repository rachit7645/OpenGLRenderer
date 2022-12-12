#include "Inputs.h"

using namespace Inputs;

InputHandler::InputHandler()
	: m_keys(SDL_GetKeyboardState(nullptr))
{
}

glm::ivec2& InputHandler::GetMousePos()
{
	return m_mousePos;
}

glm::ivec2& InputHandler::GetMouseScroll()
{
	return m_mouseScroll;
}

bool InputHandler::IsKeyPressed(SDL_Scancode key)
{
	return static_cast<bool>(m_keys[key]);
}

InputHandler& InputHandler::GetInstance()
{
	// Static storage
	static InputHandler inputs;
	// Return
	return inputs;
}