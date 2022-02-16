#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{

	struct Model
	{
		u32 vaoID;					// Vertex Attribute Object ID
		u32 eboID;					// Element Buffer Object ID
		std::vector<f32> vertices;	// Vertices
		std::vector<u32> indices;	// Indices
		std::vector<u32> vboIDs;	// Vertex Buffer Object IDs
	};

	Model CreateModel(std::vector<f32> vertices, std::vector<u32> indices);

}

#endif // MODEL_H