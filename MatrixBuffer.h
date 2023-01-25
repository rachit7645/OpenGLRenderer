#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "Camera.h"

namespace Renderer
{
	class MatrixBuffer : public UniformBuffer
	{
	public:
		// Main constructor
		MatrixBuffer();
		// Load view and inverse view matrix
		void LoadView(const Entities::Camera& camera);
		// Load projection and inverse projection matrix
		void LoadProjection(const glm::mat4& projection);
	};

	namespace Detail
	{
		// Internal representation of GPU data
		struct MatrixBufferGLSL
		{
			// Regular matrices
			alignas(16) glm::mat4 projectionMatrix;
			alignas(16) glm::mat4 viewMatrix;
			// Inverse matrices
			alignas(16) glm::mat4 invProjection;
			alignas(16) glm::mat4 invCameraView;
		};
	}
}

#endif