#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include <GL/glew.h>

#include "GLM.h"
#include "Util.h"
#include "UniformBuffer.h"

namespace Renderer
{
	class MatrixBuffer : public UniformBuffer
	{
	public:
		MatrixBuffer();
		void LoadView(const glm::mat4& view);
		void LoadProjection(const glm::mat4& projection);
	};
}

#endif