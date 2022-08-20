#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H

#include "GLM.h"
#include "UniformBuffer.h"

namespace Renderer
{
	class SharedBuffer : public UniformBuffer
	{
	public:
		SharedBuffer();

		void LoadClipPlane(const glm::vec4& clipPlane);
		void LoadSkyColor(const glm::vec4& skyColor);
	};

	namespace Detail
	{
		struct SharedBufferGLSL
		{
			glm::vec4 clipPlane;
			glm::vec4 skyColor;
		};
	}
}

#endif
