#include "Camera.h"

using namespace Entities;

Camera::Camera(Player &playerRef) : player(playerRef) {}

void Camera::Move()
{
	if (g_ToZoomCamera)
	{
		CalculateZoom();
		g_ToZoomCamera = false;
	}

	if (g_ToMoveCamera)
	{
		CalculatePitch();
		CalculateAngleAroundPlayer();
		g_ToMoveCamera = false;
	}

	f32 hDistance = distanceFromPlayer * std::cos(glm::radians(rotation.x));
	f32 vDistance = distanceFromPlayer * std::sin(glm::radians(rotation.x));
	CalculatePosition(hDistance, vDistance);
}

void Camera::CalculatePosition(f32 hDistance, f32 vDistance)
{
	f32 theta = player.rotation.y + angleAroundPlayer;
	f32 offsetX = hDistance * std::sin(glm::radians(theta));
	f32 offsetZ = hDistance * std::cos(glm::radians(theta));
	position.x = player.position.x - offsetX;
	position.z = player.position.z - offsetZ;
	position.y = player.position.y + vDistance;
	rotation.y = 180 - (player.rotation.y + angleAroundPlayer);
}

void Camera::CalculateZoom()
{
	distanceFromPlayer -= g_MouseScrollY * 0.9f;
}

void Camera::CalculatePitch()
{
	rotation.x -= g_MousePos.y * 0.1f;
	Util::Clamp<f32>(rotation.x, 5.0f, 85.0f);
}

void Camera::CalculateAngleAroundPlayer()
{
	angleAroundPlayer -= g_MousePos.x * 0.3f;
}