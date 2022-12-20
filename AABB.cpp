#include "AABB.h"

using namespace Maths;

AABB::AABB(const aiAABB& aabb)
	: min(glm::ai_cast(aabb.mMin)),
	  max(glm::ai_cast(aabb.mMax)),
	  center((max + min) * 0.5f),
	  extents(max.x - center.x, max.y - center.y, max.z - center.z)
{
}

AABB::AABB(const glm::vec3& center, f32 iI, f32 iJ, f32 iK)
	: center(center),
	  extents(iI, iJ, iK)
{
}