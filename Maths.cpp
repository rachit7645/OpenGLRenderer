#include "Maths.h"

using Entities::Camera;

glm::mat4 Maths::CreateModelMatrix
(
	const glm::vec3& translation,
	const glm::vec3& rotation,
	f32 scale
)
{
	// 1. Create an identity matrix
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

glm::mat4 Maths::CreateModelMatrixT(const glm::vec3& translation)
{
	// 1. Create an identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Translate it by translation vector
	matrix = glm::translate(matrix, translation);
	return matrix;
}

glm::mat4 Maths::CreateModelMatrixR(const glm::vec3& rotation)
{
	// 1. Create an identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Rotate it by x, y and z
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	// Return
	return matrix;
}

glm::mat4 Maths::CreateModelMatrixTS(const glm::vec3& translation, f32 scale)
{
	// 1. Create an identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Translate it by translation vector
	matrix = glm::translate(matrix, translation);
	// 4. Scale the matrix
	matrix = glm::scale(matrix, glm::vec3(scale));
	return matrix;
}

glm::mat4 Maths::CreateModelMatrix2D(const glm::vec2& translation, const glm::vec2& scale)
{
	// 1. Create an identity matrix
	glm::mat4 matrix(1.0f);
	// 2. Translate it by translation vector
	matrix = glm::translate(matrix, glm::vec3(translation, 0.0f));
	// 4. Scale the matrix
	matrix = glm::scale(matrix, glm::vec3(scale, 0.0f));
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
	// I don't use roll, so this would be an unnecessary calculation
	// matrix = glm::rotate(matrix, glm::radians(camera.m_rotation.z), glm::vec3(0, 0, 1));
	// 3. Translate by the inverse of the camera position
	matrix = glm::translate(matrix, -camera.position);
	return matrix;
}