#include "Camera.h"
#include "Window.h"

using namespace Entities;

void Camera::Move()
{
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

// Debug camera rotation
#ifdef _DEBUG
	if (g_Keys[SDL_SCANCODE_KP_4])
		yaw -= MOVE_CONSTANT * g_Delta;	
	if (g_Keys[SDL_SCANCODE_KP_6])
		yaw += MOVE_CONSTANT * g_Delta;
	if (g_Keys[SDL_SCANCODE_KP_8])
		pitch -= MOVE_CONSTANT * g_Delta;	
	if (g_Keys[SDL_SCANCODE_KP_2])
		pitch += MOVE_CONSTANT * g_Delta;
#endif
}