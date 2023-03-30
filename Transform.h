#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "GLm.h"

namespace Entities
{
    class Transform
    {
    public:
        // Default constructor
        Transform() = default;
        // Main constructor
        Transform
        (
            const glm::vec3& position,
            const glm::vec3& rotation,
            const glm::vec3& scale
        );

        // Calculates the model matrix
        glm::mat4 GetModelMatrix() const;

        // Position
        glm::vec3 position;
        // Rotation
        glm::vec3 rotation;
        // Scale
        glm::vec3 scale;
    };
}

#endif
