#ifndef FRAME_BUFFER_ATTACHMENT_H
#define FRAME_BUFFER_ATTACHMENT_H

#include <GL/glew.h>

namespace Renderer
{
	class FBOAttachment
	{
	public:
		GLint    minFilter      = 0;
		GLint    maxFilter      = 0;
		GLint    wrapMode       = 0;
		GLint    internalFormat = 0;
		GLint    format         = 0;
		GLint    dataType       = 0;
		GLenum   slot           = 0;
		GLfloat* border         = nullptr;
	};
}

#endif