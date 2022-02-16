#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>
#include "Texture.h"
#include "Util.h"

namespace Renderer
{
	struct Model
	{
		u32 vaoID;						// Vertex Attribute Object ID
		u32 eboID;						// Element Buffer Object ID
		std::vector<f32> vertices;		// Vertices
		std::vector<u32> indices;		// Indices
		std::vector<f32> textureCoords;	// Texture Coordinates
		std::vector<u32> vboIDs;		// Vertex Buffer Object IDs
		Texture texture;				// Texture
	};

	Model CreateModel(std::vector<f32>& vertices, std::vector<u32>& indices, std::vector<f32>& textureCoords, const std::string& texturePath);
}

u32 CreateVBO(u32 slot, u32 coordSize, std::vector<f32>& data);
void CreateEBO(u32* id, std::vector<u32>& data);

#endif // MODEL_H