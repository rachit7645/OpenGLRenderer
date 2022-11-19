#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"
#include "Camera.h"

namespace Renderer
{
	class SharedBuffer : public UniformBuffer
	{
	public:
		// Main constructor
		SharedBuffer();
		// Loading functions
		void LoadClipPlane(const glm::vec4& clipPlane);
		void LoadCameraPos(const Entities::Camera& camera);
		void LoadResolution(const glm::ivec2& dimensions, f32 nearPlane, f32 farPlane);
	};

	namespace Detail
	{
		// GLSL implementation struct
		struct SharedBufferGLSL
		{
			alignas(16) glm::vec4 clipPlane;
			alignas(16) glm::vec4 cameraPos;
			alignas(16) glm::vec4 resolution;
		};
	}
}

#endif
