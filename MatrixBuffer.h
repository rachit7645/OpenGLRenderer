#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include <GL/glew.h>

#include "GLM.h"
#include "Util.h"

namespace Renderer
{
	class MatrixBuffer
	{
	public:
		MatrixBuffer();
		~MatrixBuffer();

		GLuint id;
		glm::mat4 view;
		glm::mat4 projection;

		void Update();
	};
}

#endif