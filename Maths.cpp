#include "Maths.h"

using namespace Entities;

glm::mat4 Maths::CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, f32 scale)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, translation);
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(scale));
	return matrix;
}

glm::mat4 Maths::CreateViewMatrix(const Camera& camera)
{
	glm::vec3 negativeCameraPos = -camera.position;
	glm::mat4 matrix(1.0f);
	matrix = glm::rotate(matrix, glm::radians(camera.pitch), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(camera.yaw), glm::vec3(0, 1, 0));
	matrix = glm::translate(matrix, negativeCameraPos);
	return matrix;
}