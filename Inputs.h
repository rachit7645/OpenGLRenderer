#ifndef INPUTS_H
#define INPUTS_H

#include "GLM.h"
#include "Util.h"

// TODO: Add keyboard input to inputs namespace
namespace Inputs
{
	void SetMousePos(const glm::ivec2& position);
	const glm::ivec2& GetMousePos();

	void SetMouseScroll(const glm::ivec2& scroll);
	const glm::ivec2& GetMouseScroll();
}

#endif