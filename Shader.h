#ifndef SHADER_H
#define SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"

namespace Shader
{
	class ShaderProgram
	{
	public:
		// Creates a vertex and fragment shader and links em' into a program
		ShaderProgram(const std::string_view vertexPath, const std::string_view fragmentPath);
		// Geometry shader version
		ShaderProgram
		(
			const std::string_view vertexPath,
			const std::string_view fragmentPath,
			const std::string_view geometryPath
		);
		// Destructor
		virtual ~ShaderProgram();

		// Shader program ID
		GLuint programID;

		// Start shader
		void Start() const;
		// Stop shader
		void Stop() const;
	protected:
		// Query uniform location
		GLint GetUniformLocation(const char* name) const;

		// Uniform loading functions 
		void LoadUniform(GLint location, GLint value)             const;
		void LoadUniform(GLint location, GLuint value)            const;
		void LoadUniform(GLint location, GLfloat value)           const;
		void LoadUniform(GLint location, bool value)              const;
		void LoadUniform(GLint location, const glm::vec2& vector) const;
		void LoadUniform(GLint location, const glm::vec3& vector) const;
		void LoadUniform(GLint location, const glm::vec4& vector) const;
		void LoadUniform(GLint location, const glm::mat4& matrix) const;

		// Load all uniforms
		virtual void GetUniformLocations() = 0;

		// Uniform map
		std::unordered_map<std::string_view, GLint> m_uniforms;
	private:
		// Load shader from file and compile it
		GLuint LoadShader(GLenum type, const std::string_view path);
		// Check shader program errors
		void CheckProgram(const std::string_view message, GLenum type) const;
		// Check shader compilation errors
		void CheckShader(const std::string_view message, GLuint shaderID, GLenum type) const;
		// Dump shader to file
		void DumpToFile(const std::string_view path) const;
	};
}

#endif