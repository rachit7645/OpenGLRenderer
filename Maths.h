#ifndef MATHS_H
#define MATHS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Util.h"

namespace Maths
{
	glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale);
	glm::mat4 CreateViewMatrix(const Entities::Camera& camera);
}

#endif