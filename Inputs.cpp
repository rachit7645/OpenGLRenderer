#include "Inputs.h"

using namespace Engine;

Inputs::Inputs()
    : m_keys(SDL_GetKeyboardState(nullptr))
{
}

glm::ivec2& Inputs::GetMousePos()
{
    // Return mouse position
    return m_mousePos;
}

glm::ivec2& Inputs::GetMouseScroll()
{
    // Return mouse scroll
    return m_mouseScroll;
}

bool Inputs::IsKeyPressed(SDL_Scancode key)
{
    // Return key state
    return static_cast<bool>(m_keys[key]);
}

Inputs& Inputs::GetInstance()
{
    // Static storage
    static Inputs inputs;
    // Return
    return inputs;
}