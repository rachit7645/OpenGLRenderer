#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <string>
#include <memory>
#include <map>
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "Util.h"

namespace Renderer
{
	class VertexArray
	{
	public:
		GLuint id;
		std::map<const std::string, std::shared_ptr<VertexBuffer>> buffers;

		VertexArray(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords, std::vector<f32> &norms);
		VertexArray();
		~VertexArray();	
	};
}

#endif // VERTEX_ARRAY_H