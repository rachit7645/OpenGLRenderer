#include "Camera.h"

#include "ImGui.h"
#include "Inputs.h"
#include "Settings.h"

using namespace Entities;

using Engine::Settings;
using Engine::Inputs;

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
	// If main menu bar is visible
	if (ImGui::BeginMainMenuBar())
	{
		// If camera menu is visible
		if (ImGui::BeginMenu("Camera"))
		{
			// Camera position
			ImGui::Text("Position:");
			ImGui::InputFloat3("##cpos", &position[0], "%.1f");
			// Camera rotation
			ImGui::Text("Rotation:");
			ImGui::InputFloat3("##crot", &rotation[0], "%.1f");
			// Camera's distance from player
			ImGui::Text("Distance From Player:");
			ImGui::InputFloat("##cdfp", &distance, 0.0f, 0.0f, "%.1f");
			// Camera's angle around the player
			ImGui::Text("Angle Around Player:");
			ImGui::InputFloat("##carp", &m_angle, 0.0f, 0.0f, "%.1f");
			// Toggle to cap pitch
			ImGui::Checkbox("Cap Pitch", &m_capPitch);
			// End menu
			ImGui::EndMenu();
		}

		// End menu bar
		ImGui::EndMainMenuBar();
	}
}

void Camera::CalculatePosition()
{
    // Get player transform
    auto& pTransform = player->transform;

	// Calculate base and altitude
	f32 hDistance = distance * std::cos(glm::radians(rotation.x));
	f32 vDistance = distance * std::sin(glm::radians(rotation.x));

	// Calculate offsets
	f32 theta   = pTransform.rotation.y + m_angle;
	f32 offsetX = hDistance * std::sin(glm::radians(theta));
	f32 offsetZ = hDistance * std::cos(glm::radians(theta));

	// Apply modifiers
	position.x = pTransform.position.x - offsetX;
	position.z = pTransform.position.z - offsetZ;
	position.y = pTransform.position.y + vDistance;
	rotation.y = 180.0f - (pTransform.rotation.y + m_angle);
}

void Camera::CalculateZoom()
{
	// Get mouse scroll
	const auto& mouseScroll = Inputs::GetInstance().GetMouseScroll();
	// Get settings
	const auto& settings = Settings::GetInstance();
    // Calculate zoom
    distance -= settings.camera.zoomSpeed * static_cast<f32>(mouseScroll.y);
}

void Camera::CalculatePitch()
{
	// Get mouse position
	const auto& mousePos = Inputs::GetInstance().GetMousePos();
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Calculate pitch
	rotation.x -= static_cast<f32>(mousePos.y) * settings.camera.pitchSpeed;

	// Cap pitch
	if (m_capPitch)
	{
        // Clamp between minimum and maximum pitch values
		rotation.x = glm::clamp(rotation.x, settings.camera.minPitch, settings.camera.maxPitch);
	}
}

void Camera::CalculateAAP()
{
	// Get mouse pos
	const auto& mousePos = Inputs::GetInstance().GetMousePos();
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Calculate angle
	m_angle -= static_cast<f32>(mousePos.x) * settings.camera.aapSpeed;
}

glm::mat4 Camera::GetViewMatrix() const
{
    // Create an identity matrix
	glm::mat4 matrix(1.0f);
	// Rotate by X (pitch), Y (yaw) and Z (roll)
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// 3. Translate by the negation of the camera position
	matrix = glm::translate(matrix, -position);
	// Return
	return matrix;
}

void Camera::InvertPitch()
{
    // Negate pitch
	rotation.x = -rotation.x;
}

bool& Camera::GetToMoveCamera()
{
    // Return
	return m_toMoveCamera;
}

bool& Camera::GetToZoomCamera()
{
    // Return
	return m_toZoomCamera;
}