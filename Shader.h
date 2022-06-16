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
#include "Files.h"

namespace Shader
{
	constexpr auto SHADER_ERROR_BUFFER_SIZE = 4096;

	class ShaderProgram
	{
	public:
		// Creates a vertex and fragment shader and links em' into a program
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		u32 programID;

		void Start();
		void Stop();

		void BindAttribute(u32 attribute, const char* name);
		u32 GetUniformLocation(const char* name);
		virtual void GetUniformLocations() = 0;

		// Uniform loading functions 
		void LoadUniform(u32 location, GLint value);
		void LoadUniform(u32 location, GLuint value);
		void LoadUniform(u32 location, GLfloat value);
		void LoadUniform(u32 location, bool value);
		void LoadUniform(u32 location, const glm::vec3& vector);
		void LoadUniform(u32 location, const glm::vec4& vector);
		void LoadUniform(u32 location, const glm::mat4& matrix);

	private:
		// Function to load shaders from file
		u32 LoadShader(GLenum type, const std::string& path);
		void CheckShader(const std::string& message, u32 shaderID, GLenum type);
		void CheckProgram(const std::string& message, u32 programID, GLenum type);
	};
}

#endif // SHADER_H