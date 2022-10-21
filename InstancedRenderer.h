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
#include "ShadowInstancedShader.h"
#include "ShadowMap.h"

namespace Renderer
{
	class InstancedRenderer
	{
	public:
		using BufferPtr    = std::shared_ptr<InstanceBuffer>;
		using MdPtr        = std::shared_ptr<Model>;
		using EntityVector = std::vector<Entities::Entity*>;
		using Batch        = std::unordered_map<MdPtr, EntityVector>;

		InstancedRenderer
		(
			Shader::FastInstancedShader& fastShader,
			Shader::ShadowInstancedShader& shadowShader,
			Renderer::ShadowMap& shadowMap,
			BufferPtr instances
		);

		Shader::FastInstancedShader& fastShader;
		Shader::ShadowInstancedShader& shadowShader;

		Renderer::ShadowMap& shadowMap;
		BufferPtr            instances;

		void Render(const Batch& batch, Mode mode);
	private:
		void BeginRender(Mode mode);
		void EndRender(Mode mode);
		void LoadData(const EntityVector& entities);
		void PrepareMesh(const Mesh& mesh, Mode mode);
		void LoadDiffuse(const Mesh& mesh);
		void UnbindMesh();
	};
}

#endif
