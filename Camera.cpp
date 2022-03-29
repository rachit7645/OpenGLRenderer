#include "Camera.h"
#include "Window.h"

using namespace Entities;
using namespace Window;

void Camera::Move()
{
	constexpr f32 MOVE_CONSTANT = 0.2f;
	if (g_Keys[SDL_SCANCODE_W])
		position.z -= MOVE_CONSTANT * g_Delta;
	if (g_Keys[SDL_SCANCODE_A])
		position.x -= MOVE_CONSTANT * g_Delta;
	if (g_Keys[SDL_SCANCODE_S])
		position.z += MOVE_CONSTANT * g_Delta;
	if (g_Keys[SDL_SCANCODE_D])
		position.x += MOVE_CONSTANT * g_Delta;
	if (g_Keys[SDL_SCANCODE_SPACE])
		position.y += MOVE_CONSTANT * g_Delta;
	if (g_Keys[SDL_SCANCODE_LSHIFT])
		position.y -= MOVE_CONSTANT * g_Delta;	  	 	 	 	 
}