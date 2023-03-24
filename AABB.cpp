#include "AABB.h"

// Using namespaces
using namespace Maths;

AABB::AABB(const aiAABB& aabb)
	: AABB(glm::ai_cast(aabb.mMin), glm::ai_cast(aabb.mMax))
{
}

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	: min(min),
      max(max)
{
    // Create corners
    corners =
    {
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, max.z),
    };

    // Helper macro
    #define VERTEX(vertex) (vertex).x, (vertex).y, (vertex).z

    // Create vertices
    vertices =
    {
        VERTEX(corners[0]), VERTEX(corners[1]), // Line #1
        VERTEX(corners[2]), VERTEX(corners[3]), // Line #2
        VERTEX(corners[4]), VERTEX(corners[5]), // Line #3
        VERTEX(corners[6]), VERTEX(corners[7]), // Line #4

        VERTEX(corners[0]), VERTEX(corners[2]), // Line #5
        VERTEX(corners[1]), VERTEX(corners[3]), // Line #6
        VERTEX(corners[4]), VERTEX(corners[6]), // Line #7
        VERTEX(corners[5]), VERTEX(corners[7]), // Line #8

        VERTEX(corners[0]), VERTEX(corners[4]), // Line #9
        VERTEX(corners[1]), VERTEX(corners[5]), // Line #10
        VERTEX(corners[2]), VERTEX(corners[6]), // Line #11
        VERTEX(corners[3]), VERTEX(corners[7]), // Line #12
    };
}

AABB AABB::Transform(const glm::mat4& matrix) const
{
    // Corners of the new AABB
    std::array<glm::vec3, 8> newCorners = {};

    // For each new corner
    for (usize i = 0; i < corners.size(); ++i)
    {
        // Transform old corner
        newCorners[i] = glm::vec3(matrix * glm::vec4(corners[i], 1.0f));
    }

    // New minimum and maximum bounds
    glm::vec3 newMin = newCorners[0];
    glm::vec3 newMax = newMin;

    // For each corner
    for (const auto& corner : newCorners)
    {
        // Compare minimum and maximum bounds
        newMin = glm::min(newMin, corner);
        newMax = glm::max(newMax, corner);
    }

    // Return new AABB
    return AABB(newMin, newMax);
}