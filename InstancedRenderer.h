#ifndef INSTANCED_RENDERER_H
#define INSTANCED_RENDERER_H

#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "Model.h"
#include "InstancedShader.h"

namespace Renderer
{
	class InstancedRenderer
	{
	public:
		using Batch = std::unordered_map<std::shared_ptr<Model>, std::vector<Entities::Entity*>>;

		explicit InstancedRenderer(Shader::InstancedShader& shader);
		Shader::InstancedShader& shader;

		void Render(const Batch& batch);
	};
}

#endif
