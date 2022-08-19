#include "Camera.h"

#include "Imgui.h"
#include "Inputs.h"

using namespace Entities;

Camera::Camera(Player* player)
    : player(player),
      mousePos(&Inputs::GetMousePos()),
      mouseScroll(&Inputs::GetMouseScroll())
{
}

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
		CalculateAAP();
		g_ToMoveCamera = false;
	}

	CalculatePosition();
	ImGuiDisplay();
}

void Camera::ImGuiDisplay()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Camera"))
		{
			ImGui::Text("Position:");
			ImGui::InputFloat3("##cpos", &position[0], "%.1f");
			ImGui::Text("Rotation:");
			ImGui::InputFloat3("##crot", &rotation[0], "%.1f");
			ImGui::Text("Distance From Player:");
			ImGui::InputFloat("##cdfp", &distanceFromPlayer, 0.0f, 0.0f, "%.1f");
			ImGui::Text("Angle Around Player:");
			ImGui::InputFloat("##carp", &angleAroundPlayer, 0.0f, 0.0f, "%.1f");
			ImGui::Checkbox("Cap Pitch", &capPitch);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Camera::CalculatePosition()
{
	f32 hDistance = distanceFromPlayer * std::cos(glm::radians(rotation.x));
	f32 vDistance = distanceFromPlayer * std::sin(glm::radians(rotation.x));

	f32 theta   = player->rotation.y + angleAroundPlayer;
	f32 offsetX = hDistance * std::sin(glm::radians(theta));
	f32 offsetZ = hDistance * std::cos(glm::radians(theta));

	position.x = player->position.x - offsetX;
	position.z = player->position.z - offsetZ;
	position.y = player->position.y + vDistance;
	rotation.y = 180.0f - (player->rotation.y + angleAroundPlayer);
}

void Camera::CalculateZoom()
{
	// If there is no scrolling input, return
	if (mouseScroll->y == 0) return;

	// If scroll direction is positive, reduce distance from player
	if (mouseScroll->y > 0)
	{
		for (ssize_t i = 0; i < mouseScroll->y; ++i)
		{
			distanceFromPlayer -= CAMERA_ZOOM_SPEED;
		}
	}
	// If scroll direction is negative, increase distance from player
	else
	{
		for (ssize_t i = 0; i < -mouseScroll->y; ++i)
		{
			distanceFromPlayer += CAMERA_ZOOM_SPEED;
		}
	}
}

void Camera::CalculatePitch()
{
	rotation.x -= static_cast<f32>(mousePos->y * 0.1);
	if (capPitch)
	{
		rotation.x = glm::clamp<f32>(rotation.x, CAMERA_PITCH_MIN, CAMERA_PITCH_MAX);
	}
}

void Camera::CalculateAAP()
{
	angleAroundPlayer -= static_cast<f32>(mousePos->x * 0.3);
}