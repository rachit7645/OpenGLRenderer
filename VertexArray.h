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

		VertexArray(std::vector<f32>& vert, std::vector<u32>& indi, std::vector<f32>& txCoords, std::vector<f32> &norms);
		~VertexArray();

		VertexArray(const VertexArray& other);
		VertexArray(VertexArray&& other);
		VertexArray operator = (const VertexArray& other)
		{
			if (id != other.id) {
				id = other.id;
				IncRefCount();
			}	
			return *this;
		}

	private:
		void IncRefCount();	
	};
}

#endif // VERTEX_ARRAY_H