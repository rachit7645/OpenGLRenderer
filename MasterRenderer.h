#ifndef MASTER_RENDERER_H
#define MASTER_RENDERER_H

#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "EntityRenderer.h"
#include "Texture.h"
#include "Util.h"
#include "Entity.h"
#include "StaticShader.h"

namespace Renderer
{
	class MasterRenderer
	{
		public:
			MasterRenderer();

			Shader::StaticShader shader;
			Renderer::EntityRenderer renderer;
			std::unordered_map<Model*, std::vector<Entities::Entity>> entities;

			void Render(Entities::Light& light, Entities::Camera& camera);
			void ProcessEntity(Entities::Entity& entity);
	};
}

#endif