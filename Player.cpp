#include "Player.h"

#include <string>

#include "Imgui.h"
#include "MeshTextures.h"
#include "Model.h"
#include "Material.h"
#include "Inputs.h"

using namespace Entities;

using Renderer::Model;
using Renderer::MeshTextures;
using Renderer::Material;

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
	rotation.y  += turnSpeed * g_Delta;
	f32 distance = runSpeed  * g_Delta;
	position.x  += distance  * std::sin(glm::radians(rotation.y));
	position.z  += distance  * std::cos(glm::radians(rotation.y));
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
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

// FIXME: Prettify this one
void Player::CheckInputs()
{
	if (Inputs::IsKeyPressed(SDL_SCANCODE_W))
	{
		runSpeed = PLAYER_RUN_SPEED;
	}
	else if (Inputs::IsKeyPressed(SDL_SCANCODE_S))
	{
		runSpeed = -PLAYER_RUN_SPEED;
	}
	else
	{
		runSpeed = 0.0f;
	}

	if (Inputs::IsKeyPressed(SDL_SCANCODE_A))
	{
		turnSpeed = PLAYER_TURN_SPEED;
	}
	else if (Inputs::IsKeyPressed(SDL_SCANCODE_D))
	{
		turnSpeed = -PLAYER_TURN_SPEED;
	}
	else
	{
		turnSpeed = 0.0f;
	}
}