#include "Camera.h"

#include "imgui.h"
#include "Inputs.h"

using namespace Entities;

// Camera constants
constexpr auto CAMERA_ZOOM_SPEED  = 1.0f;
constexpr auto CAMERA_PITCH_SPEED = 0.1f;
constexpr auto CAMERA_AAP_SPEED   = 0.3f;
constexpr auto CAMERA_PITCH_MIN   = 0.0f;
constexpr auto CAMERA_PITCH_MAX   = 90.0f;

// Global flags
bool m_toMoveCamera = true;
bool m_toZoomCamera = true;

Camera::Camera(Player* player)
    : player(player)
{
}

void Camera::Move()
{
	// If there is mouse scroll input
	if (m_toZoomCamera)
	{
		CalculateZoom();
		m_toZoomCamera = false;
	}

	// If there is mouse movement
	if (m_toMoveCamera)
	{
		CalculatePitch();
		CalculateAAP();
		m_toMoveCamera = false;
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
	// Calculate base and altitude
	f32 hDistance = distance * std::cos(glm::radians(rotation.x));
	f32 vDistance = distance * std::sin(glm::radians(rotation.x));

	// Calculate offsets
	f32 theta   = player->rotation.y + m_angle;
	f32 offsetX = hDistance * std::sin(glm::radians(theta));
	f32 offsetZ = hDistance * std::cos(glm::radians(theta));

	// Apply modifiers
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
	rotation.x    -= static_cast<f32>(mousePos.y) * CAMERA_PITCH_SPEED;

	if (m_capPitch)
	{
		rotation.x = glm::clamp(rotation.x, CAMERA_PITCH_MIN, CAMERA_PITCH_MAX);
	}
}

void Camera::CalculateAAP()
{
	auto& mousePos = Inputs::GetMousePos();
	m_angle       -= static_cast<f32>(mousePos.x) * CAMERA_AAP_SPEED;
}

void Camera::InvertPitch()
{
	rotation.x = -rotation.x;
}

bool& Camera::GetToMoveCamera()
{
	return m_toMoveCamera;
}

bool& Camera::GetToZoomCamera()
{
	return m_toZoomCamera;
}