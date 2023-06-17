#ifndef PLANE_H
#define PLANE_H

#include "GLM.h"
#include "Util.h"

namespace Maths
{
    class Plane
    {
    public:
        // Default constructor
        Plane() = default;
        // Main constructor
        explicit Plane(const glm::vec4& equation);

        // Normalise plane equation
        void NormalizePlane();

        // Plane equation
        glm::vec4 equation = {};
        // Normal vector
        glm::vec3 normal = {};
        // Distance from origin
        f32 distance = {};
    };
}

#endif