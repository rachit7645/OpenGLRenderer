#ifndef RACHIT_GLM_H
#define RACHIT_GLM_H

// Use SSE2
#define GLM_FORCE_SSE2

// Standard vector types
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// Standard matrix types
#include <glm/mat4x4.hpp>

// Integer vector types
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int4.hpp>

// Unsigned integer vector types
#include <glm/ext/vector_uint3.hpp>

// Core operations
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>

// Matrix transformations
#include <glm/gtc/matrix_transform.hpp>

// Assimp types
#include <assimp/types.h>

namespace glm
{
    // Cast an aiVector2D to a glm::vec2
    vec2 ai_cast(const aiVector2D& vector);
    // Cast an aiVector3D to a glm::vec3
    vec3 ai_cast(const aiVector3D& vector);
}

#endif