#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

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
		f32 pitch;
		f32 yaw;
		f32 roll;
	};
}

#endif