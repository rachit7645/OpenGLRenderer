#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include "Util.h"
#include "Log.h"

namespace Shader
{

	struct ShaderProgram
	{
		u32 programID;
		u32 vertexShaderID;
		u32 fragmentShaderID;

		void Start();
		void Stop();
		void CleanUP();
	};

	ShaderProgram CreateShader(std::string vertexPath, std::string fragmentPath);

}

u32 LoadShader(GLenum type, std::string path);

#endif // SHADER_H