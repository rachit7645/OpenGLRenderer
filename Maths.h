#ifndef MATHS_H
#define MATHS_H

#include "GLM.h"
#include "Camera.h"
#include "Util.h"

namespace Maths
{
	glm::mat4 CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale);
	glm::mat4 CreateViewMatrix(const Entities::Camera& camera);
	f32 BarryCentric(const glm::vec3& vec1, const glm::vec3& vec2, const glm::vec3& vec3, const glm::vec2& position);
}

#endif