#ifndef SHADER_H
#define SHADER_H

#include <string_view>
#include <GL/glew.h>

#include "GLM.h"

namespace Shader
{
	class ShaderProgram
	{
	public:
		// Creates a vertex and fragment shader and links em\' into a program
		ShaderProgram(const std::string_view vertexPath, const std::string_view fragmentPath);
		~ShaderProgram();

		GLuint programID;

		void Start() const;
		void Stop()  const;

		void DumpToFile(const std::string_view path) const;
	protected:
		GLint GetUniformLocation(const char* name) const;

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
		GLuint LoadShader(GLenum type, const std::string_view path);
		void   CheckProgram(const std::string_view message, GLenum type)                 const;
		void   CheckShader(const std::string_view message, GLuint shaderID, GLenum type) const;
	};
}

#endif