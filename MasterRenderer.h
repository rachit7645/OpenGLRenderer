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
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"

namespace Renderer
{
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr glm::vec4 GL_CLEAR_COLOR = { 0.53f, 0.81f, 0.92f, 1.0f };
	constexpr f32 FOV = 70.0f, ASPECT_RATIO = 16.0f / 9.0f, NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;

	class MasterRenderer
	{
	public:
		MasterRenderer();

		// All shaders and renderers here 
		Shader::StaticShader shader;
		Renderer::EntityRenderer renderer;

		Shader::TerrainShader terrainShader;
		Renderer::TerrainRenderer terrainRenderer;

		// Main render function
		void Render(const Entities::Light& light, const Entities::Camera& camera);
		// Process entities into the entitiy map
		void ProcessEntity(const Entities::Entity& entity);
		// Process terrains into a vector
		void ProcessTerrain(const Terrains::Terrain& terrain);
	private:
		// Prepare framebuffer for render
		void Prepare();
		// Render entities
		void RenderEntities(const Entities::Light& light, const Entities::Camera& camera);
		// Render Terrains
		void RenderTerrains(const Entities::Light& light, const Entities::Camera& camera);

		// The entitiy map
		std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity>> entities;
		// The terrain vector
		std::vector<Terrains::Terrain> terrains;
	};
}

#endif