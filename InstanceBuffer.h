#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "ShaderStorageBuffer.h"
#include "Entity.h"
#include "GLM.h"

namespace Renderer
{
	constexpr auto NUM_MAX_ENTITIES = 500;

	namespace Detail
	{
		struct InstancedDataGLSL
		{
			alignas(16) glm::mat4 modelMatrix;
		};
	}

	class InstanceBuffer : ShaderStorageBuffer
	{
	public:
		using DataVector   = std::vector<Detail::InstancedDataGLSL>;
		using EntityVector = std::vector<Entities::Entity*>;

		InstanceBuffer();

		void LoadInstanceData(const EntityVector& entities);
	private:
		DataVector GenerateData(const EntityVector& entities);
		GLsizeiptr GetSize(const DataVector& data);
	};
}

#endif
