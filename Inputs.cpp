#include "Inputs.h"

glm::ivec2 mousePos;
glm::ivec2 mouseScroll;

void Inputs::SetMousePos(const glm::ivec2& position)
{
	mousePos = position;
}

const glm::ivec2& Inputs::GetMousePos()
{
	return mousePos;
}

void Inputs::SetMouseScroll(const glm::ivec2& scroll)
{
	mouseScroll = scroll;
}

const glm::ivec2& Inputs::GetMouseScroll()
{
	return mouseScroll;
}