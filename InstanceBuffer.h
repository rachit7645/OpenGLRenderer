#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <vector>
#include <GL/glew.h>

#include "ShaderStorageBuffer.h"
#include "Entity.h"
#include "GLM.h"
#include "Util.h"

namespace Renderer
{
	// Maximum entities allowed in one batch
	constexpr usize NUM_MAX_ENTITIES = 512;

	namespace Detail
	{
		// GLSL representation of per-instance data
		struct InstancedDataGLSL
		{
			// Model matrix
			alignas(16) glm::mat4 modelMatrix;
			// Normal matrix
			alignas(16) glm::mat4 normalMatrix;
		};

		// GLSL representation of instance buffer
		struct InstancedBufferGLSL
		{
			// Array of instance data
			alignas(16) InstancedDataGLSL instances[NUM_MAX_ENTITIES];
		};
	}

	class InstanceBuffer : ShaderStorageBuffer
	{
	public:
		// Usings
		using DataVector   = std::vector<Detail::InstancedDataGLSL>;
		using EntityVector = std::vector<Entities::Entity*>;

		// Main constructor
		InstanceBuffer();

		// Bind buffer
		void Bind() const;
		// Unbind buffer
		void Unbind() const;

		// Load instance data
		void LoadInstanceData(const EntityVector& entities);
	private:
		// Create instance data
		DataVector GenerateData(const EntityVector& entities);
		// Get size of instance data
		usize GetSize(const EntityVector& entities);
	};
}

#endif
