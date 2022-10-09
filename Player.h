#ifndef PLAYER_H
#define PLAYER_H

#include "GLM.h"
#include "Entity.h"
#include "Util.h"
#include "Model.h"

namespace Entities
{
	class Player : public Entity
	{
	public:
		Player
		(
			MdPtr model,
			const glm::vec3& position,
			const glm::vec3& rotation,
			f32 scale
		);

		void Move();
	private:
		f32 m_runSpeed  = 0.0f;
		f32 m_turnSpeed = 0.0f;

		void CheckInputs();
		void ImGuiDisplay();
	};
}

#endif