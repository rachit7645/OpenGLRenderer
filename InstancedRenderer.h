#ifndef INSTANCED_RENDERER_H
#define INSTANCED_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "Entity.h"
#include "Model.h"
#include "InstanceBuffer.h"
#include "FastInstancedShader.h"
#include "RenderConstants.h"
#include "ShadowShader.h"
#include "ShadowMap.h"
#include "IBLMaps.h"

namespace Renderer
{
	class InstancedRenderer
	{
	public:
		// Usings
		using BufferPtr    = std::shared_ptr<InstanceBuffer>;
		using MdPtr        = std::shared_ptr<Model>;
		using EntityVector = std::vector<Entities::Entity*>;
		using Batch        = std::unordered_map<MdPtr, EntityVector>;

		// Main constructor
		InstancedRenderer
		(
			Shader::FastInstancedShader& fastShader,
			Shader::ShadowShader& shadowShader,
			Renderer::ShadowMap& shadowMap,
			Renderer::IBLMaps& iblMaps,
			BufferPtr instances
		);

		// Shaders
		Shader::FastInstancedShader& fastShader;
		Shader::ShadowShader&        shadowShader;

		// Other data
		Renderer::ShadowMap& shadowMap;
		Renderer::IBLMaps&   iblMaps;
		BufferPtr            instances;

		// Render batch
		void Render(const Batch& batch, Mode mode);
	private:
		// Begin rendering
		void BeginRender(Mode mode);
		// End rendering
		void EndRender(Mode mode);
		// Load instance data
		void LoadData(const EntityVector& entities);
		// Prepare mesh
		void PrepareMesh(const Mesh& mesh, Mode mode);
		// Load textures
		void LoadTextures(const Mesh& mesh);
		// Load IBL textures
		void LoadIBLTextures();
		// Unbind mesh
		void UnbindMesh();
	};
}

#endif
