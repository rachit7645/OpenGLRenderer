#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Util.h"
#include "Log.h"

namespace Shader
{
	class ShaderProgram
	{
	public:	
		u32 programID;
		u32 vertexShaderID;
		u32 fragmentShaderID;

		void Start();
		void Stop();
		void CleanUP();

		u32 GetUniformLocation(const std::string& name) const;
		void LoadInt(u32 location, u32 value) const;
		void LoadFloat(u32 location, f32 value) const;
		void LoadBool(u32 location, bool value) const;
		void LoadVector(u32 location, const glm::vec3& vector) const;
		void LoadMatrix(u32 location, const glm::mat4& matrix) const;
	};
	
	ShaderProgram CreateShader(std::string vertexPath, std::string fragmentPath);
	u32 LoadShader(GLenum type, std::string path);
}

#endif // SHADER_H