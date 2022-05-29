#include "Player.h"

using namespace Entities;
using Renderer::Model;

Player::Player(std::shared_ptr<Model> &model, const glm::vec3 &position, const glm::vec3 &rotation, f32 scale)
	: Entity(model, position, rotation, scale) {}

void Player::Move()
{
	CheckInputs();
	rotation.y += turnSpeed * g_Delta;
	f32 distance = runSpeed * g_Delta;
	position.x += distance * std::sin(glm::radians(rotation.y));
	position.z += distance * std::cos(glm::radians(rotation.y));
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