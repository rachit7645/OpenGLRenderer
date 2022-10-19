#ifndef G_BUFFER_RENDERER_H
#define G_BUFFER_RENDERER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "GBufferShader.h"
#include "InstanceBuffer.h"

namespace Renderer
{
	class GBufferRenderer
	{
	public:
		using BufferPtr    = std::shared_ptr<InstanceBuffer>;
		using MdPtr        = std::shared_ptr<Model>;
		using EntityVector = std::vector<Entities::Entity*>;
		using Batch        = std::unordered_map<MdPtr, EntityVector>;

		GBufferRenderer(Shader::GBufferShader& shader);
		Shader::GBufferShader& shader;

		void Render(const Batch& batch);
	private:
		void BeginRender();
		void EndRender();
		void LoadData(const EntityVector& entities);
		void PrepareMesh(const Mesh& mesh);
		void LoadDiffuse(const Mesh& mesh);
		void LoadSpecular(const Mesh& mesh);
		void UnbindMesh();

		BufferPtr m_buffer;
	};
}

#endif
