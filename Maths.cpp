#include "Maths.h"

using Entities::Camera;

glm::mat4 Maths::CreateModelMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale)
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

glm::mat4 Maths::CreateModelMatrix(const glm::vec3& rotation)
{
	// 1. Create a identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Rotate it by x, y and z
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	// Return
	return matrix;
}

glm::mat4 Maths::CreateViewMatrix(const Camera& camera)
{
	// 1. Create an identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Rotate by x (pitch), y (yaw) and optionally z (roll)
	matrix = glm::rotate(matrix, glm::radians(camera.rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(camera.rotation.y), glm::vec3(0, 1, 0));
	// I don't use roll, so this would be unnecessary calculation
	// matrix = glm::rotate(matrix, glm::radians(camera.rotation.z), glm::vec3(0, 0, 1));
	// 3. Translate by the inverse of the camera position
	matrix = glm::translate(matrix, -camera.position);
	return matrix;
}

// No idea how this works
// But it does work
f32 Maths::BarryCentric(const glm::vec3& vec1, const glm::vec3& vec2, const glm::vec3& vec3, const glm::vec2& position)
{
	f32 det = (vec2.z - vec3.z) * (vec1.x - vec3.x) + (vec3.x - vec2.x) * (vec1.z - vec3.z);
	f32 l1 = ((vec2.z - vec3.z) * (position.x - vec3.x) + (vec3.x - vec2.x) * (position.y - vec3.z)) / det;
	f32 l2 = ((vec3.z - vec1.z) * (position.x - vec3.x) + (vec1.x - vec3.x) * (position.y - vec3.z)) / det;
	f32 l3 = 1.0f - l1 - l2;
	return l1 * vec1.y + l2 * vec2.y + l3 * vec3.y;
}