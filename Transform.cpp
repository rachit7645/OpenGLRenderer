#include "Transform.h"

// Using namespaces
using namespace Entities;

Transform::Transform
(
    const glm::vec3& position,
    const glm::vec3& rotation,
    const glm::vec3& scale
)
    : position(position),
      rotation(rotation),
      scale(scale)
{
}

glm::mat4 Transform::GetModelMatrix() const
{
    // Create an identity matrix
    glm::mat4 matrix(1.0f);
    // Translate it by position vector
    matrix = glm::translate(matrix, position);
    // Rotate it by the rotation angles
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // Scale the matrix
    matrix = glm::scale(matrix, scale);
    // Return
    return matrix;
}