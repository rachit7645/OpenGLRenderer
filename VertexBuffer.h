#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <map>
#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{
	class VertexBuffer
	{
	public:
		GLuint id;

		VertexBuffer(u32 slot, u32 coordSize, std::vector<f32>& data);
		VertexBuffer(std::vector<u32>& data);
		VertexBuffer();
		~VertexBuffer();

		VertexBuffer(const VertexBuffer& other);
		VertexBuffer(VertexBuffer&& other);
		VertexBuffer operator = (const VertexBuffer& other)
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

#endif // VERTEX_BUFFER_H