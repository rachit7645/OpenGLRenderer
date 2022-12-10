#include "Player.h"

#include "Inputs.h"
#include "Settings.h"

// Using namespaces
using namespace Entities;

// Usings
using Renderer::Model;
using Renderer::MeshTextures;
using Engine::Settings;

Player::Player()
	: Entity(nullptr, glm::vec3(0.0f), glm::vec3(0.0f), 0.0f)
{
}

Player::Player
(
	MdPtr model,
	const glm::vec3& position,
	const glm::vec3& rotation,
	f32 scale
)
	: Entity(std::move(model), position, rotation, scale)
{
}

void Player::Move()
{
	CheckInputs();

	rotation.y  += m_turnSpeed * g_Delta;
	f32 distance = m_runSpeed  * g_Delta;

	position.x  += distance  * std::sin(glm::radians(rotation.y));
	position.z  += distance  * std::cos(glm::radians(rotation.y));

	m_turnSpeed = 0.0f;
	m_runSpeed  = 0.0f;

	ImGuiDisplay();
}

void Player::ImGuiDisplay()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Player"))
		{
			// Player attribs
			ImGui::Text("Position:");
			ImGui::InputFloat3("##pos", &position[0], "%.1f");
			ImGui::Text("Rotation:");
			ImGui::InputFloat3("##rot", &rotation[0], "%.1f");
			ImGui::Text("Scale:");
			ImGui::InputFloat("##scl", &scale, 0.0f, 0.0f, "%.1f");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Player::CheckInputs()
{
	// Get settings
	const auto& settings = Settings::GetInstance();

	if (Inputs::IsKeyPressed(SDL_SCANCODE_W))
	{
		m_runSpeed = settings.player.runSpeed;
	}
	else if (Inputs::IsKeyPressed(SDL_SCANCODE_S))
	{
		m_runSpeed = -settings.player.runSpeed;
	}

	if (Inputs::IsKeyPressed(SDL_SCANCODE_A))
	{
		m_turnSpeed = settings.player.turnSpeed;
	}
	else if (Inputs::IsKeyPressed(SDL_SCANCODE_D))
	{
		m_turnSpeed = -settings.player.turnSpeed;
	}
}