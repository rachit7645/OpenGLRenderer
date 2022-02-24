#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <GL/glew.h>
#include "Model.h"

namespace Renderer
{
	class GLRenderer
	{
	public:
		void Prepare();
		void Render(const Model& model);
	};
}

#endif