#include "GLM.h"

glm::vec2 glm::ai_cast(const aiVector2D& vector)
{
    return glm::vec2(vector.x, vector.y);
}

glm::vec3 glm::ai_cast(const aiVector3D& vector)
{
    return glm::vec3(vector.x, vector.y, vector.z);
}