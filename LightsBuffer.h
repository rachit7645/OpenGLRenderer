#ifndef LIGHTS_BUFFER_H
#define LIGHTS_BUFFER_H

#include <GL/glew.h>

#include "Util.h"
#include "GLM.h"
#include "UniformBuffer.h"
#include "Light.h"

namespace Renderer
{
	class LightsBuffer : public UniformBuffer
	{
	public:
		LightsBuffer();
		void LoadLight(const Entities::Light& light);
	};
}

#endif