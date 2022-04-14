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
	constexpr auto VERTEX_SHADER_PATH = "res/shaders/vertexShader.glsl",
		FRAGMENT_SHADER_PATH = "res/shaders/fragmentShader.glsl";
	constexpr auto TERRAIN_VERTEX_SHADER_PATH = "res/shaders/terrainVertexShader.glsl",
		TERRAIN_FRAGMENT_SHADER_PATH = "res/shaders/terrainFragmentShader.glsl";

	constexpr f32 FOV = 70.0f, ASPECT_RATIO = 16.0f / 9.0f, NEAR_PLANE = 0.1f, FAR_PLANE = 1000.0f;
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	constexpr GLclampf RED = 0.49f, GREEN = 89.0f, BLUE = 0.98f, ALPHA = 1.0f;

	class MasterRenderer
	{
	public:
		MasterRenderer();

		// All shaders and renderers here 
		Shader::StaticShader shader;
		Renderer::EntityRenderer renderer;

		Shader::TerrainShader terrainShader;
		Renderer::TerrainRenderer terrainRenderer;

		// Prepare framebuffer for drawing
		void Prepare();		
		// Main render function
		void Render(Entities::Light &light, Entities::Camera &camera);
		// Process entities into THE MAP
		void ProcessEntity(Entities::Entity &entity);
		void ProcessTerrain(Terrains::Terrain &terrain);
	private:
		// THE MAP
		std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity>> entities;
		std::vector<Terrains::Terrain> terrains;
	};
}

#endif