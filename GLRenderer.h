#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <GL/glew.h>
#include "Entity.h"

namespace Renderer
{
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr GLclampf RED = 0.25f, GREEN = 0.25f, BLUE = 0.25f, ALPHA = 1.0f;

	class GLRenderer
	{
	public:
		void Prepare();
		void Render(const Entities::Entity& entity);
	};
}

#endif