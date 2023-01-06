#ifndef FRAME_BUFFER_ATTACHMENT_H
#define FRAME_BUFFER_ATTACHMENT_H

#include <GL/glew.h>

namespace Renderer
{
	class FBOAttachment
	{
	public:
		// Minimum texture filter
		GLint minFilter = 0;
		// Maximum texture filter
		GLint maxFilter = 0;
		// Texture wrapping mode
		GLint wrapMode = 0;
		// Internal texture format
		GLint intFormat = 0;
		// Texture format
		GLint format = 0;
		// Data type
		GLint dataType = 0;
		// Attachment slot
		GLenum slot = 0;
		// Border, if any
		GLfloat* border = nullptr;
	};
}

#endif