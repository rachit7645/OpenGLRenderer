#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#include "GLM.h"
#include "Util.h"

namespace Entities
{
	constexpr auto MOVE_CONSTANT = 1.0f;

	class Camera
	{
	public:	
		Camera(const glm::vec3 &position) : position{ position } {}
		void Move();

		glm::vec3 position;
		f32 pitch = 0.0f;
		f32 yaw = 0.0f;
		f32 roll = 0.0f;
	};
}

#endif