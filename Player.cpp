#include "Player.h"

#include "Inputs.h"

using namespace Entities;

using Renderer::Model;
using Renderer::MeshTextures;

constexpr auto PLAYER_RUN_SPEED  = 45.0f;
constexpr auto PLAYER_TURN_SPEED = 160.0f;

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
	if (Inputs::IsKeyPressed(SDL_SCANCODE_W))
	{
		m_runSpeed = PLAYER_RUN_SPEED;
	}
	else if (Inputs::IsKeyPressed(SDL_SCANCODE_S))
	{
		m_runSpeed = -PLAYER_RUN_SPEED;
	}

	if (Inputs::IsKeyPressed(SDL_SCANCODE_A))
	{
		m_turnSpeed = PLAYER_TURN_SPEED;
	}
	else if (Inputs::IsKeyPressed(SDL_SCANCODE_D))
	{
		m_turnSpeed = -PLAYER_TURN_SPEED;
	}
}