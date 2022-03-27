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
	constexpr f32 FOV = 70.0f;
	constexpr f32 ASPECT_RATIO = 16.0f / 9.0f; 
	constexpr f32 NEAR_PLANE = 0.1f;
	constexpr f32 FAR_PLANE = 1000.0f;
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr GLclampf RED = 0.25f, GREEN = 0.25f, BLUE = 0.25f, ALPHA = 1.0f;

	class GLRenderer
	{
	public:
		GLRenderer(Shader::StaticShader &shader);

		void Prepare();
		void Render(const Entities::Entity &entity, Shader::StaticShader &shader);

		glm::mat4 projectionMatrix;
	private:
		glm::mat4 CreateProjectionMatrix();		
	};
}

#endif