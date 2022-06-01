#ifndef PLAYER_H
#define PLAYER_H

#include "GLM.h"
#include "Imgui.h"
#include "Entity.h"
#include "Util.h"
#include "Model.h"
#include "Terrain.h"

namespace Entities
{
	constexpr auto PLAYER_RUN_SPEED = 45.0f;
	constexpr auto PLAYER_TURN_SPEED = 160.0f;
	constexpr auto PLAYER_GRAVITY = -20.0f;

	class Player : public Entity
	{
	public:
		Player(std::shared_ptr<Renderer::Model>& model, const glm::vec3 &position, const glm::vec3 &rotation, f32 scale);
		void Move(const Terrains::Terrain* terrain);
	private:
		f32 runSpeed = 0.0f;
		f32 turnSpeed = 0.0f;
		f32 terrainHeight = 0.0f;

		void CheckInputs();
		void Gravity(const Terrains::Terrain* terrain);
		void ImGuiDisplay();
	};
}

#endif