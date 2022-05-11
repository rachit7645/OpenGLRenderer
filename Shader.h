#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "GLM.h"
#include "Util.h"
#include "Log.h"

namespace Shader
{
	class ShaderProgram
	{
	public:
		// Creates a vertex and fragment shader and links em' into a program
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		u32 programID;

		void Start();
		void Stop();

		void BindAttribute(u32 attribute, const char* name) const;
		u32 GetUniformLocation(const char* name) const;
		virtual void GetUniformLocations() = 0;

		// Uniform loading functions 
		void LoadInt(u32 location, s32 value) const;
		void LoadFloat(u32 location, f32 value) const;
		void LoadBool(u32 location, bool value) const;
		void LoadVector(u32 location, const glm::vec3& vector) const;
		void LoadVector(u32 location, const glm::vec4& vector) const;		
		void LoadMatrix(u32 location, const glm::mat4& matrix) const;

	private:
		// Function to load shaders from file
		u32 LoadShader(GLenum type, const std::string& path);
		void CheckShader(const std::string &message, u32 shaderID, GLenum type, Error error);
		void CheckProgram(u32 programID, GLenum type, Error error);
	};
}

#endif // SHADER_H