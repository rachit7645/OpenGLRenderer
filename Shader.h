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
		// Creates a vertex and fragment shader and links em\' into a program
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		GLuint programID;

		void Start() const;
		void Stop()  const;

		GLint GetUniformLocation(const char* name) const;
		void  DumpToFile(const std::string& path)  const;

		// Uniform loading functions 
		void LoadUniform(GLint location, GLint value)             const;
		void LoadUniform(GLint location, GLuint value)            const;
		void LoadUniform(GLint location, GLfloat value)           const;
		void LoadUniform(GLint location, bool value)              const;
		void LoadUniform(GLint location, const glm::vec3& vector) const;
		void LoadUniform(GLint location, const glm::vec4& vector) const;
		void LoadUniform(GLint location, const glm::mat4& matrix) const;

		virtual void GetUniformLocations() = 0;
	private:
		// Function to load shaders from file
		GLuint LoadShader(GLenum type, const std::string& path);
		void   CheckProgram(const std::string& message, GLenum type)                 const;
		void   CheckShader(const std::string& message, GLuint shaderID, GLenum type) const;
	};
}

#endif // SHADER_H