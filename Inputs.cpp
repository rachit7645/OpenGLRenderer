#include "Inputs.h"

glm::ivec2 mousePos;
glm::ivec2 mouseScroll;

glm::ivec2& Inputs::MousePos()
{
	return mousePos;
}

glm::ivec2& Inputs::MouseScroll()
{
	return mouseScroll;
}