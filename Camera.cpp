#include "Camera.h"

using namespace Entities;

#include "Window.h"

void Camera::Move()
{
	constexpr f32 MOVE_CONSTANT = 0.002f;
	if (key == SDLK_w)
		position.z -= MOVE_CONSTANT;
	if (key == SDLK_a)
		position.x -= MOVE_CONSTANT;
	if (key == SDLK_s)
		position.z += MOVE_CONSTANT;
	if (key == SDLK_d)
		position.x += MOVE_CONSTANT;
	key == 0;	 	 	 	 
}