#include "Camera.h"

using namespace Entities;

Camera::Camera(Player& playerRef) : player(playerRef) {}

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
	ImGuiDisplay();
}

void Camera::ImGuiDisplay()
{
	ImGui::Begin("Camera");
	ImGui::Text("Position:\nX: %.2f\nY: %.2f\nZ: %.2f", position.x, position.y, position.z);
	ImGui::Text("Rotation:\nPitch: %.2f\nYaw: %.2f\nRoll: %.2f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("Distance From Player: %.2f\nAngle Around Player: %.2f\nCamera Speed Constant: %.2f",
		distanceFromPlayer, angleAroundPlayer, CAMERA_SPEED);
	ImGui::Text("Mouse Position:\nX: %d\nY: %d\nMouse Scroll:\nX: %d\nY: %d",
		g_MousePos.x, g_MousePos.y, g_MouseScroll.x, g_MouseScroll.y);
	ImGui::Checkbox("Cap Pitch", &capPitch);
	ImGui::End();
}

void Camera::CalculatePosition(f32 hDistance, f32 vDistance)
{
	f32 theta = player.rotation.y + angleAroundPlayer;
	f32 offsetX = hDistance * std::sin(glm::radians(theta));
	f32 offsetZ = hDistance * std::cos(glm::radians(theta));
	position.x = player.position.x - offsetX;
	position.z = player.position.z - offsetZ;
	position.y = player.position.y + vDistance;
	rotation.y = 180.0f - (player.rotation.y + angleAroundPlayer);
}

void Camera::CalculateZoom()
{
	// If there is no scrolling input, return
	if (g_MouseScroll.y == 0) return;

	// If scroll direction is positive, reduce distance from player
	if (g_MouseScroll.y > 0)
	{
		for (ssize_t i = 0; i < g_MouseScroll.y; ++i)
		{
			distanceFromPlayer -= CAMERA_ZOOM_SPEED;
		}
	}
	// If scroll direction is negative, increase distance from player
	else
	{
		for (ssize_t i = 0; i < -g_MouseScroll.y; ++i)
		{
			distanceFromPlayer += CAMERA_ZOOM_SPEED;
		}
	}
}

void Camera::CalculatePitch()
{
	// Pitch clamp values, edit as you like :)
	// Or disable it in the camera options
	constexpr auto CAMERA_PITCH_MIN = 0.0f;
	constexpr auto CAMERA_PITCH_MAX = 90.0f;

	rotation.x -= g_MousePos.y * 0.1f;
	if (capPitch)
	{
		Util::Clamp<f32>(rotation.x, CAMERA_PITCH_MIN, CAMERA_PITCH_MAX);
	}
}

void Camera::CalculateAngleAroundPlayer()
{
	angleAroundPlayer -= g_MousePos.x * 0.3f;
}