#include "Camera.h"

#include "imgui.h"
#include "Inputs.h"
#include "Settings.h"

using namespace Entities;

using Engine::Settings;

// Global flags
bool m_toMoveCamera = true;
bool m_toZoomCamera = true;

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
	// Get mouse scroll
	auto& mouseScroll = Inputs::GetMouseScroll();
	// Get settings
	const auto& settings = Settings::GetInstance();

	if (mouseScroll.y > 0)
	{
		for (ssize i = 0; i < mouseScroll.y; ++i)
		{
			distance -= settings.camera.zoomSpeed;
		}
	}
	else
	{
		for (ssize i = 0; i < -mouseScroll.y; ++i)
		{
			distance += settings.camera.zoomSpeed;
		}
	}
}

void Camera::CalculatePitch()
{
	// Get mouse position
	auto& mousePos = Inputs::GetMousePos();
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Calculate pitch
	rotation.x -= static_cast<f32>(mousePos.y) * settings.camera.pitchSpeed;

	// Cap pitch
	if (m_capPitch)
	{
		rotation.x = glm::clamp(rotation.x, settings.camera.minPitch, settings.camera.maxPitch);
	}
}

void Camera::CalculateAAP()
{
	// Get mouse pos
	auto& mousePos = Inputs::GetMousePos();
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Calculate angle
	m_angle -= static_cast<f32>(mousePos.x) * settings.camera.aapSpeed;
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