#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <string>
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
		std::map<const std::string, VertexBuffer> buffers;

		VertexArray(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords);
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray &operator=(const VertexArray&) = delete;
		VertexArray &operator=(VertexArray &&other)
		{
			if (this != &other)
			{
				Delete();
				id = other.id;
				other.id = 0;
			}
			return *this;
		}
		VertexArray(VertexArray&& other);

	private:
		void Delete();	
	};
}

#endif // VERTEX_ARRAY_H