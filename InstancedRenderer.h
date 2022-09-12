#ifndef INSTANCED_RENDERER_H
#define INSTANCED_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "Entity.h"
#include "Model.h"
#include "InstancedShader.h"
#include "InstanceBuffer.h"

namespace Renderer
{
	class InstancedRenderer
	{
	public:
		using BufferPtr    = std::shared_ptr<InstanceBuffer>;
		using MdPtr        = std::shared_ptr<Model>;
		using EntityVector = std::vector<Entities::Entity*>;
		using Batch        = std::unordered_map<MdPtr, EntityVector>;

		explicit InstancedRenderer(Shader::InstancedShader& shader);
		Shader::InstancedShader& shader;

		void Render(const Batch& batch);
	private:
		void BeginRender();
		void EndRender();
		void LoadData(const EntityVector& entities);
		void PrepareMesh(const Mesh& mesh);
		void UnbindMesh();

		BufferPtr m_buffer;
	};
}

#endif
