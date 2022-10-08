#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "Camera.h"
#include "Light.h"

namespace Renderer
{
	class MatrixBuffer : public UniformBuffer
	{
	public:
		MatrixBuffer();

		void LoadView(const Entities::Camera& camera);
		void LoadProjection(const glm::mat4& projection);

		void LoadLightProjection(const glm::mat4& projection);
		void LoadLightView(const glm::mat4& view);
	};

	namespace Detail
	{
		struct MatrixBufferGLSL
		{
			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			// TODO: Move to LightsBuffer
			glm::mat4 lightProj;
			// TODO: Move to LightsBuffer
			glm::mat4 lightView;
		};
	}
}

#endif