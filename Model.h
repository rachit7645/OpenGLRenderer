#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <map>
#include <GL/glew.h>
#include "Texture.h"
#include "Util.h"

namespace Renderer
{
	struct Model
	{
		Model(std::vector<f32>& m_vertices, std::vector<u32>& m_indices, std::vector<f32>& m_textureCoords, const std::string& texturePath);
		u32 vaoID;								 // Vertex Attribute Object ID
		u32 eboID;								 // Element Buffer Object ID
		std::vector<f32> vertices;				 // Vertices
		std::vector<u32> indices;				 // Indices
		std::vector<f32> textureCoords;			 // Texture Coordinates
		std::map<const std::string, u32> vboIDs; // Vertex Buffer Object IDs
		Texture texture;						 // Texture
	};
}

u32 CreateVBO(u32 slot, u32 coordSize, std::vector<f32>& data);
u32 CreateEBO(std::vector<u32>& data);

#endif // MODEL_H