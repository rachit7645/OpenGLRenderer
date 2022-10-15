#ifndef SHADOW_MATRIX_BUFFER_H
#define SHADOW_MATRIX_BUFFER_H

#include <vector>
#include <array>
#include <GL/glew.h>

#include "GLM.h"
#include "UniformBuffer.h"
#include "Util.h"
#include "GL.h"

namespace Renderer
{
	constexpr usize SHADOW_MAX_FRUSTUMS = 16;

	class ShadowBuffer : public UniformBuffer
	{
	public:
		using Mat4s = std::vector<glm::mat4>;

		ShadowBuffer();

		void LoadMatrices(const Mat4s& matrices);
		void LoadDistances(const std::vector<f32>& distances);
	};

	namespace Detail
	{
		struct ShadowBufferGLSL
		{
			alignas(16) GL::IntGLSL   cascadeCount;
			alignas(16) glm::mat4     matrices[SHADOW_MAX_FRUSTUMS];
			alignas(16) GL::FloatGLSL cascadeDistances[SHADOW_MAX_FRUSTUMS];
		};
	}
}

#endif
