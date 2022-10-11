#ifndef SHADOW_MATRIX_BUFFER_H
#define SHADOW_MATRIX_BUFFER_H

#include <vector>

#include "GLM.h"
#include "UniformBuffer.h"
#include "Util.h"

namespace Renderer
{
	constexpr usize SHADOW_MAX_FRUSTUMS = 16;

	class ShadowMatrixBuffer : public UniformBuffer
	{
	public:
		using Mat4s = std::vector<glm::mat4>;

		ShadowMatrixBuffer();

		void LoadMatrices(const Mat4s& matrices);
	};

	namespace Detail
	{
		struct ShadowMatrixBufferGLSL
		{
			alignas(16) glm::mat4 matrices[SHADOW_MAX_FRUSTUMS];
		};
	}
}

#endif
