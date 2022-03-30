#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Util.h"
#include "Maths.h"
#include "Entity.h"
#include "StaticShader.h"

namespace Renderer
{
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr GLclampf RED = 0.25f, GREEN = 0.25f, BLUE = 0.25f, ALPHA = 1.0f;

	class GLRenderer
	{
	public:
		void Prepare();
		void Render(Entities::Entity& entity, Shader::StaticShader& shader);
	};
}

#endif