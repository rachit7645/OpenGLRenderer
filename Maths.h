#ifndef MATHS_H
#define MATHS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Util.h"

namespace Maths
{
	constexpr auto FOV = 70.0f, ASPECT_RATIO = 16.0f / 9.0f, NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;
	
	glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale);
	glm::mat4 CreateViewMatrix(const Entities::Camera& camera);
	glm::mat4 CreateProjectionMatrix();
}

#endif