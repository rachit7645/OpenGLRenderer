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
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr GLclampf RED = 0.25f, GREEN = 0.25f, BLUE = 0.25f, ALPHA = 1.0f;

	class EntityRenderer
	{
	public:
		EntityRenderer(Shader::StaticShader& sh);

		Shader::StaticShader& shader;

		void Prepare();
		void Render(std::unordered_map<Model*, std::vector<Entities::Entity>>& entities);
		void PrepareModel(Model* model);
		void UnbindModel();
		void PrepareInstance(Entities::Entity& entity);
	};
}

#endif