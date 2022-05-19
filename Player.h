#ifndef PLAYER_H
#define PLAYER_H

#include "GLM.h"
#include "Entity.h"
#include "Util.h"
#include "Model.h"

namespace Entities
{
	constexpr auto PLAYER_RUN_SPEED = 45.0f;
	constexpr auto PLAYER_TURN_SPEED = 160.0f;

	class Player : public Entity
	{
	public:
		Player(std::shared_ptr<Renderer::Model>& model, const glm::vec3 &position, const glm::vec3 &rotation, f32 scale);
		void Move();
	private:
		f32 runSpeed = 0.0f;
		f32 turnSpeed = 0.0f;

		void CheckInputs();
	};
}

#endif