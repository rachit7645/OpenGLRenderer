#include <glm/glm.hpp>
#include "Util.h"

namespace Entities
{
	class Camera
	{
	public:
		Camera() { position = glm::vec3(0.0f); }

		glm::vec3 position;
		f32 pitch;
		f32 yaw;
		f32 roll;
	};
}