#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Util.h"
#include "Maths.h"
#include "Entity.h"
#include "StaticShader.h"

namespace Renderer
{
	constexpr f32 FOV = 70.0f, ASPECT_RATIO = 16.0f / 9.0f, NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr GLclampf RED = 0.25f, GREEN = 0.25f, BLUE = 0.25f, ALPHA = 1.0f;

	class EntityRenderer
	{
	public:
		EntityRenderer(Shader::StaticShader& sh);
		Shader::StaticShader& shader;

		// Prepare framebuffer for drawing
		void Prepare();
		// Render THE MAP
		void Render(std::unordered_map<Model*, std::vector<Entities::Entity>> entities);
		// Prepare each model for render
		void PrepareModel(Model* model);
		// Prepare each instance
		void PrepareInstance(const Entities::Entity& entity);
		// Unbind the model
		void UnbindModel();
	};
}

#endif