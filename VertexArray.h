#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include <unordered_map>
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "Util.h"

namespace Renderer
{
	class VertexArray
	{
	public:
		GLuint id;
		std::unordered_map<const char*, std::shared_ptr<VertexBuffer>> buffers;

		VertexArray(const std::vector<f32>& vertices, const std::vector<u32>& indices,
			const std::vector<f32>& txCoords, const std::vector<f32>& normals);
		VertexArray(const std::vector<f32>& vertices);	
		VertexArray();
		~VertexArray();
	};
}

#endif // VERTEX_ARRAY_H