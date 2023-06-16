#ifndef RENDER_CONSTANTS_H
#define RENDER_CONSTANTS_H

#include <GL/glew.h>

#include "Util.h"
#include "GLM.h"

namespace Renderer
{
	// Projection constants
	constexpr f32 FOV          = 70.0f;
	constexpr f32 ASPECT_RATIO = static_cast<f32>(16.0 / 9.0);
	constexpr f32 NEAR_PLANE   = 0.1f;
	constexpr f32 FAR_PLANE    = 1024.0f;

	// Render modes
	enum class Mode
	{
		Deferred,
		Fast,
		Shadow,
        OmniShadow
	};
}

#endif
