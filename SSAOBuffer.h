#ifndef SSAO_BUFFER_H
#define SSAO_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "GL.h"
#include "Util.h"

namespace Renderer
{
	// Constants
	constexpr usize MAX_KERNEL_SIZE = 64;

	class SSAOBuffer : public UniformBuffer
	{
	public:
		// Main constructor
		SSAOBuffer();

		// Load sample kernels
		void LoadKernels(const std::vector<glm::vec3>& kernels);
	};

	namespace Detail
	{
		struct SSAOBufferGLSL
		{
			alignas(16) GL::Int   kernelSize;
			alignas(16) glm::vec4 samples[MAX_KERNEL_SIZE];
		};
	}
}

#endif
