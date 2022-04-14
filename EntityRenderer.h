#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <unordered_map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Util.h"
#include "Maths.h"
#include "Entity.h"
#include "StaticShader.h"

namespace Renderer
{
	class EntityRenderer
	{
	public:
		EntityRenderer(Shader::StaticShader& sh);
		Shader::StaticShader& shader;

		// Render THE MAP
		void Render(std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity>> entities);
		// Prepare each model for render
		void PrepareModel(std::shared_ptr<Model> model);
		// Prepare each instance
		void PrepareInstance(Entities::Entity& entity);
		// Unbind the model
		void UnbindModel();
	};
}

#endif