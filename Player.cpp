#include "Player.h"

using namespace Entities;

using Renderer::Model;
using Terrains::Terrain;

Player::Player(std::shared_ptr<Model>& model, const glm::vec3& position, const glm::vec3& rotation, f32 scale)
	: Entity(model, position, rotation, scale) {}

void Player::Move(const Terrain* terrain)
{
	CheckInputs();
	rotation.y += turnSpeed * g_Delta;
	f32 distance = runSpeed * g_Delta;
	position.x += distance * std::sin(glm::radians(rotation.y));
	position.z += distance * std::cos(glm::radians(rotation.y));
	Gravity(terrain);
	ImGuiDisplay();
}

void Player::ImGuiDisplay()
{
	ImGui::Begin("Player");
	ImGui::Text("Position:\nX: %.2f\nY: %.2f\nZ: %.2f", position.x, position.y, position.z);
	ImGui::Text("Rotation:\nX: %.2f\nY: %.2f\nZ: %.2f", rotation.x, rotation.y, rotation.z);
	ImGui::Text("Run Speed: %.2f\nTurn Speed: %.2f\nGravity Constant: %.2f", runSpeed, turnSpeed, PLAYER_GRAVITY);
	ImGui::Text("Terrain Height: %.2f", terrainHeight);
	ImGui::End();
}

void Player::Gravity(const Terrain* terrain)
{
	if (terrain == nullptr)
	{
		terrainHeight = 0.0f;
	}
	else
	{
		terrainHeight = terrain->GetHeight(glm::vec2(position.x, position.z));
	}

	if (position.y > terrainHeight)
	{
		position.y += PLAYER_GRAVITY * g_Delta;
	}

	if (position.y < terrainHeight)
	{
		position.y = terrainHeight;
	}
}

void Player::CheckInputs()
{
	if (g_Keys[SDL_SCANCODE_W])
	{
		runSpeed = PLAYER_RUN_SPEED;
	}
	else if (g_Keys[SDL_SCANCODE_S])
	{
		runSpeed = -PLAYER_RUN_SPEED;
	}
	else
	{
		runSpeed = 0.0f;
	}

	if (g_Keys[SDL_SCANCODE_A])
	{
		turnSpeed = PLAYER_TURN_SPEED;
	}
	else if (g_Keys[SDL_SCANCODE_D])
	{
		turnSpeed = -PLAYER_TURN_SPEED;
	}
	else
	{
		turnSpeed = 0.0f;
	}
}