#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
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

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer &operator=(const VertexBuffer&) = delete;
		VertexBuffer &operator=(VertexBuffer &&other)
		{
			if (this != &other)
			{
				Delete();
				id = other.id;
				other.id = 0;
			}
			return *this;
		}
		VertexBuffer(VertexBuffer&& other);

	private:
		void Delete();		
	};
}

#endif // VERTEX_BUFFER_H