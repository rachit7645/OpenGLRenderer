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
#include "Skybox.h"
#include "SkyboxRenderer.h"
#include "SkyboxShader.h"
#include "MatrixBuffer.h"
#include "LightsBuffer.h"

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

		Shader::SkyboxShader skyboxShader;
		Renderer::SkyboxRenderer skyboxRenderer;

		// Main render function
		void Render(const std::vector<Entities::Light>& lights, const Entities::Camera& camera);
		// Process entities into the entitiy map
		void ProcessEntity(const Entities::Entity& entity);
		// Process terrains into a vector
		void ProcessTerrain(const Terrains::Terrain& terrain);
		// Process skybox into a class member
		void ProcessSkybox(const Entities::Skybox& skybox);
	private:
		// Prepare framebuffer for render
		void Prepare();
		// Update buffers, etc.
		void Update(const std::vector<Entities::Light>& pLights, const Entities::Camera& camera);
		// Render entities
		void RenderEntities();
		// Render Terrains
		void RenderTerrains();
		// Render the skybox
		void RenderSkybox();

		// The entitiy map
		std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity>> entities;
		// The terrain vector
		std::vector<Terrains::Terrain> terrains;
		// The Skybox
		Entities::Skybox skybox;
		// Uniform buffer objects
		std::shared_ptr<MatrixBuffer> matrices;
		std::shared_ptr<LightsBuffer> lights;
	};
}

#endif