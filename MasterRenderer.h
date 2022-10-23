#ifndef MASTER_RENDERER_H
#define MASTER_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "GLM.h"
#include "Entity.h"
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
#include "FastInstancedShader.h"
#include "RenderConstants.h"
#include "ShadowMap.h"
#include "ShadowInstancedShader.h"
#include "GBuffer.h"
#include "GBufferShader.h"
#include "GBufferRenderer.h"
#include "LightingShader.h"
#include "LightingRenderer.h"
#include "InstanceBuffer.h"

// TODO: Create separate shadow renderer

namespace Renderer
{
	class MasterRenderer
	{
	private:
		// Note: These be initialised before everything
		// Framebuffers
		Renderer::ShadowMap       m_shadowMap;
		Waters::WaterFrameBuffers m_waterFBOs;
		Renderer::GBuffer         m_gBuffer;
		// Instances Shader Storage Buffer
		std::shared_ptr<InstanceBuffer> m_instances;
	public:
		MasterRenderer();

		Shader::FastInstancedShader   fastInstancedShader;
		Shader::ShadowInstancedShader shadowInstancedShader;
		Renderer::InstancedRenderer   instancedRenderer;

		Shader::GBufferShader      gShader;
		Renderer::GBufferRenderer  gRenderer;
		Shader::LightingShader     lightShader;
		Renderer::LightingRenderer lightRenderer;

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
		// Finish frame
		void EndFrame();
		// Render the water
		void RenderWaters(const std::vector<Waters::WaterTile>& waters);
		// Render water fbos
		void RenderWaterFBOs(const std::vector<Waters::WaterTile>& waters, Entities::Camera& camera);
		// Render shadows
		void RenderShadows(const Entities::Camera& camera, const Entities::Light& light);
		// Render global buffer
		void RenderGBuffer(const Entities::Camera& camera);
		// Render lighting pass
		void RenderLighting(const Entities::Camera& camera);
		// Render skybox
		void RenderSkybox();
		// Render the guis
		void RenderGUIs(const std::vector<GUI>& guis);
		// Copy depth
		void CopyDepth();
		// Process entities into the entity map
		void ProcessEntity(Entities::Entity& entity);
		// Process a vector of entities
		void ProcessEntities(std::vector<Entities::Entity>& entities);
	private:
		// Render Scene
		void RenderScene(const Entities::Camera& camera, Mode mode, const glm::vec4& clipPlane = glm::vec4(0.0f));
		// Prepare render
		void Prepare(const Entities::Camera& camera, const glm::vec4& clipPlane = glm::vec4(0.0f));
		// Render entities
		void RenderEntities(Mode mode);
		// Draw ImGui Windows
		void RenderImGui();

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