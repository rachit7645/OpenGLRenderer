#ifndef MATHS_H
#define MATHS_H

#include "GLM.h"
#include "Camera.h"
#include "Util.h"

namespace Maths
{
	// Create normal model matrix
	glm::mat4 CreateModelMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale);
	// Create model matrix with only translation
	glm::mat4 CreateModelMatrixT(const glm::vec3& translation);
	// Create model matrix with only rotation
	glm::mat4 CreateModelMatrixR(const glm::vec3& rotation);
	// Create view matrix from camera
	glm::mat4 CreateViewMatrix(const Entities::Camera& camera);
	// BarryCentric equation
	f32 BarryCentric
	(
		const glm::vec3& vec1,
		const glm::vec3& vec2,
		const glm::vec3& vec3,
		const glm::vec2& position
	);
}

#endif