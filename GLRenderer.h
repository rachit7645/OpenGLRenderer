#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <GL/glew.h>
#include "Model.h"

namespace Renderer
{

	void Prepare();
	void Render(Model model);

}

#endif