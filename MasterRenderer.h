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
#include "InstancedShader.h"
#include "FastInstancedShader.h"
#include "RenderConstants.h"
#include "ShadowFrameBuffer.h"

namespace Renderer
{
	class MasterRenderer
	{
	public:
		MasterRenderer();

		Shader::InstancedShader     instancedShader;
		Shader::FastInstancedShader fastInstancedShader;
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
		void RenderScene(const Entities::Camera& camera, const glm::vec4& clipPlane = glm::vec4(0.0f), Mode mode = Mode::Normal);
		// Render the water
		void RenderWaters(const std::vector<Waters::WaterTile>& waters, const Waters::WaterFrameBuffers& waterFBOs);
		// Render shadows
		void RenderShadows(const Renderer::ShadowFrameBuffer& shadowFBO, const Entities::Camera& camera);
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
		void RenderEntities(Mode mode);
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