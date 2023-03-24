#ifndef AABB_H
#define AABB_H

#include <array>
#include <vector>

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
        AABB(const glm::vec3& min, const glm::vec3& max);

        // Transform current AABB into a new one
		AABB Transform(const glm::mat4& matrix) const;

        // Minimum bound
		glm::vec3 min = {};
		// Maximum bound
		glm::vec3 max = {};
        // AABB corners
        std::array<glm::vec3, 8> corners = {};
        // AABB vertices for debugging
        std::vector<f32> vertices = {};
	};
}

#endif