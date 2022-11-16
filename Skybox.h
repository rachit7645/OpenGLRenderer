#ifndef SKYBOX_H
#define SKYBOX_H

#include <memory>

#include "VertexArray.h"
#include "Texture.h"

namespace Entities
{
	class Skybox
	{
	public:
		// Usings
		using VAO   = std::shared_ptr<Renderer::VertexArray>;
		using TxPtr = std::shared_ptr<Renderer::Texture>;
		// Main constructor
		Skybox();
		// Copy constructor
		explicit Skybox(TxPtr texture);
		// Data
		VAO   vao;
		TxPtr cubeMap;
	private:
		VAO LoadVAO();
	};
}


#endif