#include "Camera.h"

#include "Imgui.h"
#include "Inputs.h"

using namespace Entities;

constexpr auto CAMERA_ZOOM_SPEED = 1.0f;
constexpr auto CAMERA_PITCH_MIN  = 0.0f;
constexpr auto CAMERA_PITCH_MAX  = 90.0f;

Camera::Camera(Player* player)
    : player(player)
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
			ImGui::InputFloat("##cdfp", &distance, 0.0f, 0.0f, "%.1f");
			ImGui::Text("Angle Around Player:");
			ImGui::InputFloat("##carp", &m_angle, 0.0f, 0.0f, "%.1f");
			ImGui::Checkbox("Cap Pitch", &m_capPitch);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Camera::CalculatePosition()
{
	f32 hDistance = distance * std::cos(glm::radians(rotation.x));
	f32 vDistance = distance * std::sin(glm::radians(rotation.x));

	f32 theta   = player->rotation.y + m_angle;
	f32 offsetX = hDistance * std::sin(glm::radians(theta));
	f32 offsetZ = hDistance * std::cos(glm::radians(theta));

	position.x = player->position.x - offsetX;
	position.z = player->position.z - offsetZ;
	position.y = player->position.y + vDistance;
	rotation.y = 180.0f - (player->rotation.y + m_angle);
}

void Camera::CalculateZoom()
{
	auto& mouseScroll = Inputs::GetMouseScroll();

	// If scroll direction is positive, reduce distance from player
	if (mouseScroll.y > 0)
	{
		for (ssize i = 0; i < mouseScroll.y; ++i)
		{
			distance -= CAMERA_ZOOM_SPEED;
		}
	}
	// If scroll direction is negative, increase distance from player
	else
	{
		for (ssize i = 0; i < -mouseScroll.y; ++i)
		{
			distance += CAMERA_ZOOM_SPEED;
		}
	}
}

void Camera::CalculatePitch()
{
	auto& mousePos = Inputs::GetMousePos();
	rotation.x    -= static_cast<f32>(mousePos.y * 0.1);

	if (m_capPitch)
	{
		rotation.x = glm::clamp<f32>(rotation.x, CAMERA_PITCH_MIN, CAMERA_PITCH_MAX);
	}
}

void Camera::CalculateAAP()
{
	auto& mousePos     = Inputs::GetMousePos();
	m_angle -= static_cast<f32>(mousePos.x * 0.3);
}

void Camera::InvertPitch()
{
	rotation.x = -rotation.x;
}
