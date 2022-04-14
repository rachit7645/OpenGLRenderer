#include "Maths.h"

using namespace Entities;

glm::mat4 Maths::CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale)
{
	// 1. Create a identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Translate it by translation vector
	matrix = glm::translate(matrix, translation);
	// 3. Rotate it by x, y and z
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	// 4. Scale the matrix
	matrix = glm::scale(matrix, glm::vec3(scale));
	return matrix;
}

glm::mat4 Maths::CreateViewMatrix(const Camera& camera)
{
	glm::vec3 negativeCameraPos = -camera.position;
	// 1. Create an identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Rotate by x (pitch), y (yaw) and optionally z (roll)
	matrix = glm::rotate(matrix, glm::radians(camera.pitch), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(camera.yaw), glm::vec3(0, 1, 0));
	// 3. Translate by the inverse of the camera position
	matrix = glm::translate(matrix, negativeCameraPos);
	return matrix;
}