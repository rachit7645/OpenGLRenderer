#include "AABB.h"

// Using namespaces
using namespace Maths;

// This is honestly pretty slow

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
    return {newMin, newMax};
}