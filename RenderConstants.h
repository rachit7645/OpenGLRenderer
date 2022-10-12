#ifndef RENDER_CONSTANTS_H
#define RENDER_CONSTANTS_H

#include <GL/glew.h>

#include "Util.h"
#include "GLM.h"

namespace Renderer
{
	constexpr glm::vec4  GL_CLEAR_COLOR = {0.0f,  0.0f,  0.0f,  1.0f};
	constexpr glm::vec4  GL_SKY_COLOR   = {0.53f, 0.81f, 0.92f, 1.0f};
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	constexpr f32 FOV          = 70.0f;
	constexpr f32 ASPECT_RATIO = 16.0 / 9.0;
	constexpr f32 NEAR_PLANE   = 0.1f;
	constexpr f32 FAR_PLANE    = 300.0f;

	// TODO: Store current mode in renderers
	enum class Mode
	{
		Normal,
		Fast,
		Shadow
	};
}

#endif
