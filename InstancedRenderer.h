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
		using Batch     = std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity*>>;
		using BufferPtr = std::shared_ptr<InstanceBuffer>;

		explicit InstancedRenderer(Shader::InstancedShader& shader);
		Shader::InstancedShader& shader;

		void Render(const Batch& batch);
	private:
		BufferPtr buffer;
	};
}

#endif
