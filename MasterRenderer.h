#ifndef MASTER_RENDERER_H
#define MASTER_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include "EntityRenderer.h"
#include "Texture.h"
#include "Util.h"
#include "Entity.h"
#include "StaticShader.h"

namespace Renderer
{
	constexpr auto VERTEX_SHADER_PATH = "res/shaders/vertexShader.glsl", FRAGMENT_SHADER_PATH = "res/shaders/fragmentShader.glsl";

	class MasterRenderer
	{
	public:
		MasterRenderer();

		// All shaders and renderers here 
		Shader::StaticShader shader;
		Renderer::EntityRenderer renderer;

		// Main render function
		void Render(Entities::Light &light, Entities::Camera &camera);
		// Process entities into THE MAP
		void ProcessEntity(Entities::Entity &entity);
	private:
		// THE MAP
		std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity>> entities;
	};
}

#endif