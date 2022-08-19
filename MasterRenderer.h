#ifndef MASTER_RENDERER_H
#define MASTER_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <GL/glew.h>

#include "EntityRenderer.h"
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
#include "GUIRenderer.h"
#include "GUIShader.h"
#include "GUI.h"
#include "WaterShader.h"
#include "WaterRenderer.h"
#include "WaterTile.h"
#include "Player.h"

namespace Renderer
{
	constexpr glm::vec4  GL_CLEAR_COLOR = {0.0f, 0.0f, 0.0f, 1.0f};
	constexpr glm::vec4  GL_SKY_COLOR   = {0.53f, 0.81f, 0.92f, 1.0f};
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	constexpr f32 FOV           = 70.0f;
	constexpr f32 ASPECT_RATIO  = 16.0 / 9.0;
	constexpr f32 NEAR_PLANE    = 0.1f;
	constexpr f32 FAR_PLANE     = 1000.0f;

	class MasterRenderer
	{
	public:
		MasterRenderer();

		// All shaders and renderers here 
		Shader::StaticShader     shader;
		Renderer::EntityRenderer renderer;

		Shader::TerrainShader     terrainShader;
		Renderer::TerrainRenderer terrainRenderer;

		Shader::SkyboxShader     skyboxShader;
		Renderer::SkyboxRenderer skyboxRenderer;

		Shader::GUIShader     guiShader;
		Renderer::GUIRenderer guiRenderer;

		Shader::WaterShader     waterShader;
		Renderer::WaterRenderer waterRenderer;

		// Render Scene
		void RenderScene
		(
			std::vector<Entities::Entity>& entities,
			std::vector<Terrains::Terrain>& terrains,
			const std::vector<Entities::Light>& lights,
			const Entities::Camera& camera,
			Entities::Player& player
		);
		// Main render function
		void Render(const std::vector<Entities::Light>& lights, const Entities::Camera& camera);
		// Render the water
		void RenderWaters(const std::vector<Waters::WaterTile>& waters);
		// Render the guis
		void RenderGUIs(const std::vector<GUI>& guis);
		// Process entities into the entity map
		void ProcessEntity(Entities::Entity& entity);
		// Process a vector of entities
		void ProcessEntities(std::vector<Entities::Entity>& entities);
		// Process terrains into a vector
		void ProcessTerrain(Terrains::Terrain& terrain);
		// Process a vector of terrains
		void ProcessTerrains(std::vector<Terrains::Terrain>& terrains);
	private:
		// Prepare framebuffer for render
		void Prepare();
		// Update buffers, etc.
		void Update(const std::vector<Entities::Light>& lights, const Entities::Camera& camera);
		// Render entities
		void RenderEntities();
		// Render Terrains
		void RenderTerrains();
		// Render the skybox
		void RenderSkybox();

		// The entity map
		std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity*>> m_entities;
		// The terrain vector
		std::vector<Terrains::Terrain*> m_terrains;
		// The Skybox
		Entities::Skybox m_skybox;
		// Matrix Uniform Buffer
		std::shared_ptr<MatrixBuffer> m_matrices;
		// Lights uniform Buffer
		std::shared_ptr<LightsBuffer> m_lights;
	};
}

#endif