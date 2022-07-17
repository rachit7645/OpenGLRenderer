#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

#include "GLM.h"

namespace Shader
{
	constexpr auto SHADER_ERROR_BUFFER_SIZE = 4096;

	class ShaderProgram
	{
	public:
		// Creates a vertex and fragment shader and links em' into a program
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		GLuint programID;

		void Start();
		void Stop();

		void BindAttribute(GLuint attribute, const char* name);
		GLuint GetUniformLocation(const char* name);
		void DumpToFile(const std::string& path);

		// Uniform loading functions 
		void LoadUniform(GLuint location, GLint value);
		void LoadUniform(GLuint location, GLuint value);
		void LoadUniform(GLuint location, GLfloat value);
		void LoadUniform(GLuint location, bool value);
		void LoadUniform(GLuint location, const glm::vec3& vector);
		void LoadUniform(GLuint location, const glm::vec4& vector);
		void LoadUniform(GLuint location, const glm::mat4& matrix);

		virtual void GetUniformLocations() = 0;
	private:
		// Function to load shaders from file
		GLuint  LoadShader(GLenum type, const std::string& path);
		void CheckShader(const std::string& message, GLuint shaderID, GLenum type);
		void CheckProgram(const std::string& message, GLuint programID, GLenum type);
	};
}

#endif // SHADER_H