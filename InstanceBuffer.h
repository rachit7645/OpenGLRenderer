#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "ShaderStorageBuffer.h"
#include "Entity.h"
#include "GLM.h"

namespace Renderer
{
	constexpr auto NUM_MAX_ENTITIES = 512;

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

		void Bind()   const;
		void Unbind() const;

		void LoadInstanceData(const EntityVector& entities);
	private:
		DataVector GenerateData(const EntityVector& entities);
		GLsizeiptr GetSize(const EntityVector& entities, const DataVector& data);
	};
}

#endif
