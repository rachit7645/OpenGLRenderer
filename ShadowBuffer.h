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
    // Max shadow cascade length
	constexpr usize SHADOW_MAX_FRUSTUMS = 16;

	class ShadowBuffer : public UniformBuffer
	{
	public:
        // Usings
		using Mat4s = std::vector<glm::mat4>;

        // Representation og GLSL data
        struct ALIGN_GLSL_STD140 ShadowBufferGLSL
        {
            GL::Int   cascadeCount;
            glm::mat4 matrices[SHADOW_MAX_FRUSTUMS];
            GL::Float cascadeDistances[SHADOW_MAX_FRUSTUMS];
        };

        // Main constructor
		ShadowBuffer();

        // Load shadow matrices
		void LoadMatrices(const Mat4s& matrices);
        // Load shadow distances
		void LoadDistances(const std::vector<f32>& distances);
	};
}

#endif
