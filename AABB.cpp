#include "AABB.h"

// Using namespaces
using namespace Maths;

AABB::AABB(const aiAABB& aabb)
	: min(glm::ai_cast(aabb.mMin)),
	  max(glm::ai_cast(aabb.mMax)),
  	  center((max + min) * 0.5f),
  	  extents(glm::abs(max - center)),
      size(2.0f * extents)
{
}

AABB::AABB(const glm::vec3& center, const glm::vec3& extents)
	: center(center),
      extents(extents),
      size(2.0f * extents)
{
	// Calculate minimum
    min = center - extents;
    // Calculate maximum
    max = center + extents;
}

AABB AABB::Transform(const glm::mat4& model) const
{
    // Get new matrix
    auto M = glm::mat3(model);

    // Transform extents
    auto X = M * glm::vec3(extents.x, 0.0f, 0.0f);
    auto Y = M * glm::vec3(0.0f, extents.y, 0.0f);
    auto Z = M * glm::vec3(0.0f, 0.0f, extents.z);

    // Calculate variables
    auto newExtents = glm::abs(X) + glm::abs(Y) + glm::abs(Z);
    auto newCenter  = glm::vec3(model * glm::vec4(center, 1.0f));

    // Return new AABB
    return AABB(newCenter, newExtents);
}

glm::vec3 AABB::GetPositive(const glm::vec3& normal) const
{
    // Set result to minimum value
    glm::vec3 result = min;

    // Check X
    if (normal.x > 0.0f)
    {
        // Increase result in X direction
        result.x += size.x;
    }

    // Check Y
    if (normal.y > 0.0f)
    {
        // Increase result in Y direction
        result.y += size.y;
    }

    // Check Z
    if (normal.z > 0.0f)
    {
        // Increase result in Z direction
        result.z += size.z;
    }

    // Return
    return result;
}

glm::vec3 AABB::GetNegative(const glm::vec3& normal) const
{
    // Set result to minimum value
    glm::vec3 result = min;

    // Check X
    if (normal.x < 0.0f)
    {
        // Increase result in X direction
        result.x += size.x;
    }

    // Check Y
    if (normal.y < 0.0f)
    {
        // Increase result in Y direction
        result.y += size.y;
    }

    // Check Z
    if (normal.z < 0.0f)
    {
        // Increase result in Z direction
        result.z += size.z;
    }

    // Return
    return result;
}