#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace Renderer
{
	class RenderManager
	{
	public:
		// Usings
		using EntityVec   = std::vector<Entities::Entity>;
		using DirLights   = std::vector<Entities::DirectionalLight>;
		using PointLights = std::vector<Entities::PointLight>;
		using SpotLights  = std::vector<Entities::SpotLight>;
		using WaterTiles  = std::vector<Waters::WaterTile>;
		using GUIs        = std::vector<Renderer::GUI>;
		using EntityPtrs  = std::vector<Entities::Entity*>;
		using MdPtr       = std::shared_ptr<Model>;
		using Batch       = std::unordered_map<MdPtr, EntityPtrs>;
		using TxPtr       = std::shared_ptr<Texture>;
		// Main constructor
		RenderManager();
	private:
		// Framebuffers
		Renderer::ShadowMap       m_shadowMap;
		Waters::WaterFrameBuffers m_waterFBOs;
		Renderer::GBuffer         m_gBuffer;
		Renderer::IBLMaps         m_iblMaps;
		// Instances Shader Storage Buffer
		std::shared_ptr<InstanceBuffer> m_instances;
		// Instanced renderer and shaders
		Shader::FastInstancedShader   m_fastInstancedShader;
		Shader::ShadowInstancedShader m_shadowInstancedShader;
		Renderer::InstancedRenderer   m_instancedRenderer;
		// Deferred renderers and shaders
		Shader::GBufferShader      m_gShader;
		Renderer::GBufferRenderer  m_gRenderer;
		Shader::LightingShader     m_lightShader;
		Renderer::LightingRenderer m_lightRenderer;
		// Skybox renderer and shader
		Shader::SkyboxShader     m_skyboxShader;
		Renderer::SkyboxRenderer m_skyboxRenderer;
		// Gui renderer and shader
		Shader::GUIShader     m_guiShader;
		Renderer::GUIRenderer m_guiRenderer;
		// Water renderer and shader
		Shader::WaterShader     m_waterShader;
		Renderer::WaterRenderer m_waterRenderer;
		// The entity map
		Batch m_entities;
		// The Skybox
		Entities::Skybox m_skybox;
		// Matrix Uniform Buffer
		std::shared_ptr<MatrixBuffer> m_matrices;
		// Lights Uniform Buffer
		std::shared_ptr<LightsBuffer> m_lights;
		// Shared Uniform Buffer
		std::shared_ptr<SharedBuffer> m_shared;
		// GPU Info
		std::string m_glVendor;
		std::string m_glRenderer;
		std::string m_glVersion;
		std::string m_glslVersion;
		// GPU Memory Info
		bool isGPUMemoryInfo = false;
		f32  totalMemory     = 0.0f;
		// Current framebuffer
		TxPtr m_currentFBO;
	public:
		// Prepare frame
		void BeginFrame
		(
			EntityVec& entities,
			const DirLights& dirLights,
			const PointLights& pointLights,
			const SpotLights& spotLights,
			Entities::Player& player
		);
		// Finish frame
		void EndFrame();
		// Render shadows
		void RenderShadows(const Entities::Camera& camera, const glm::vec3& lightPos);
		// Render the waters
		void RenderWaters(const WaterTiles& waters);
		// Render water fbos
		void RenderWaterFBOs(const WaterTiles& waters, Entities::Camera& camera);
		// Render global buffer
		void RenderGBuffer(const Entities::Camera& camera);
		// Render lighting pass
		void RenderLighting(const Entities::Camera& camera);
		// Render skybox
		void RenderSkybox();
		// Render the guis
		void RenderGUIs(const GUIs& guis);
		// Copy depth
		void CopyDepth();
		// Process entities into the entity map
		void ProcessEntity(Entities::Entity& entity);
		// Process a vector of entities
		void ProcessEntities(EntityVec& entities);
	private:
		// Render Scene
		void RenderScene(const Entities::Camera& camera, Mode mode, const glm::vec4& clipPlane = glm::vec4(0.0f));
		// Prepare render
		void Prepare(const Entities::Camera& camera, const glm::vec4& clipPlane = glm::vec4(0.0f));
		// Render entities
		void RenderEntities(Mode mode);
		// Draw ImGui Windows
		void RenderImGui();
	};
}

#endif