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
		// Global constructor
		AABB(const glm::vec3& center, f32 iI, f32 iJ, f32 iK);
		// Minimum bound
		glm::vec3 min = {};
		// Maximum bound
		glm::vec3 max = {};
		// Center
		glm::vec3 center = {};
		// Extents
		glm::vec3 extents = {};
	};
}

#endif
