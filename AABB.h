#ifndef AABB_H
#define AABB_H

#include <assimp/aabb.h>

#include "GLM.h"
#include "Util.h"

namespace Maths
{
	class AABB
	{
	public:
		// Main constructor
		explicit AABB(const aiAABB& aabb);
        // Transform constructor
        AABB(const glm::vec3& center, const glm::vec3& extents);

        // Transform current AABB into a new one
		AABB Transform(const glm::mat4& model) const;

        // Get positive intersection
        glm::vec3 GetPositive(const glm::vec3& normal) const;
        // Get negative intersection
        glm::vec3 GetNegative(const glm::vec3& normal) const;

        // Minimum bound
		glm::vec3 min = {};
		// Maximum bound
		glm::vec3 max = {};
		// Center
		glm::vec3 center = {};
		// Extents
		glm::vec3 extents = {};
        // Size
        glm::vec3 size = {};
	};
}

#endif