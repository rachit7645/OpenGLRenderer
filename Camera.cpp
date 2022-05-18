#include "Camera.h"

using namespace Entities;

void Camera::Move()
{
	if (g_Keys[SDL_SCANCODE_UP])
		position.z -= CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_LEFT])
		position.x -= CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_DOWN])
		position.z += CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_RIGHT])
		position.x += CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_SPACE])
		position.y += CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_LSHIFT])
		position.y -= CAMERA_SPEED * g_Delta;

	// Debug camera rotation
#ifdef _DEBUG
	if (g_Keys[SDL_SCANCODE_KP_4])
		yaw -= CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_KP_6])
		yaw += CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_KP_8])
		pitch -= CAMERA_SPEED * g_Delta;
	if (g_Keys[SDL_SCANCODE_KP_2])
		pitch += CAMERA_SPEED * g_Delta;
#endif
}