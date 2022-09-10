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
#include "SharedBuffer.h"
#include "WaterFrameBuffers.h"
#include "InstancedRenderer.h"
#include "InstancedShader.h"

namespace Renderer
{
	constexpr glm::vec4  GL_CLEAR_COLOR = {0.0f,  0.0f,  0.0f,  1.0f};
	constexpr glm::vec4  GL_SKY_COLOR   = {0.53f, 0.81f, 0.92f, 1.0f};
	constexpr GLbitfield GL_CLEAR_FLAGS = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	constexpr f32 FOV          = 70.0f;
	constexpr f32 ASPECT_RATIO = 16.0 / 9.0;
	constexpr f32 NEAR_PLANE   = 0.1f;
	constexpr f32 FAR_PLANE    = 1000.0f;

	class MasterRenderer
	{
	public:
		MasterRenderer();

		Shader::StaticShader     shader;
		Renderer::EntityRenderer renderer;

		Shader::InstancedShader     instancedShader;
		Renderer::InstancedRenderer instancedRenderer;

		Shader::SkyboxShader     skyboxShader;
		Renderer::SkyboxRenderer skyboxRenderer;

		Shader::GUIShader     guiShader;
		Renderer::GUIRenderer guiRenderer;

		Shader::WaterShader     waterShader;
		Renderer::WaterRenderer waterRenderer;

		// Prepare frame
		void BeginFrame
		(
			std::vector<Entities::Entity>& entities,
			const std::vector<Entities::Light>& lights,
			Entities::Player& player
		);
		// Render Scene
		void RenderScene(const Entities::Camera& camera, const glm::vec4& clipPlane = glm::vec4(0.0f));
		// Render the water
		void RenderWaters(const std::vector<Waters::WaterTile>& waters, const Waters::WaterFrameBuffers& waterFBOs);
		// Render the guis
		void RenderGUIs(const std::vector<GUI>& guis);
		// Finish frame
		void EndFrame();
		// Process entities into the entity map
		void ProcessEntity(Entities::Entity& entity);
		// Process a vector of entities
		void ProcessEntities(std::vector<Entities::Entity>& entities);
	private:
		// Prepare render
		void Prepare(const Entities::Camera& camera, const glm::vec4& clipPlane);
		// Render entities
		void RenderEntities();
		// Render the skybox
		void RenderSkybox();

		// The entity map
		std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity*>> m_entities;
		// The Skybox
		Entities::Skybox m_skybox;
		// Matrix Uniform Buffer
		std::shared_ptr<MatrixBuffer> m_matrices;
		// Lights Uniform Buffer
		std::shared_ptr<LightsBuffer> m_lights;
		// Shared Uniform Buffer
		std::shared_ptr<SharedBuffer> m_shared;
	};
}

#endif