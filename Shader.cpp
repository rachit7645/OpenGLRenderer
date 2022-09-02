#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <fmt/format.h>

#include "Util.h"
#include "Log.h"
#include "Files.h"

using namespace Shader;

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
	programID = glCreateProgram();
	u32 vertexShaderID   = LoadShader(GL_VERTEX_SHADER,   vertexPath);
	u32 fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	CheckProgram(fmt::format("Shader link failed for: {}, {}", vertexPath, fragmentPath), GL_LINK_STATUS);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glValidateProgram(programID);
	CheckProgram(fmt::format("Shader validation failed for: {}, {}", vertexPath, fragmentPath), GL_VALIDATE_STATUS);
}

void ShaderProgram::DumpToFile(const std::string& path) const
{
	GLint length;
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length);

	GLenum format;
	std::vector<char> binary(length);
	glGetProgramBinary
	(
		programID,
		static_cast<GLsizei>(binary.size()),
		&length,
		&format,
		reinterpret_cast<void*>(binary.data())
	);

	std::ofstream file(path);
	file.write(binary.data(), length);

	LOG_DEBUG("Dumping Shader to: ", path, "\n");
}

u32 ShaderProgram::LoadShader(GLenum type, const std::string& path)
{
	LOG_INFO("Loading shader: {}\n", path);
	std::ifstream fs(Files::GetResourceDirectory() + path, std::ios::in);

	if (!fs.is_open())
	{
		LOG_ERROR("Unable to open shader: ", path);
	}
	std::string content = std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());

	u32 shaderID = glCreateShader(type);
	const GLchar* cstr = content.c_str();
	glShaderSource(shaderID, 1, &cstr, nullptr);
	glCompileShader(shaderID);
	CheckShader(fmt::format("Shader Compilation Failed: {}", path), shaderID, GL_COMPILE_STATUS);

	return shaderID;
}

void ShaderProgram::CheckShader(const std::string& message, u32 shaderID, GLenum type) const
{
	GLint status;
	glGetShaderiv(shaderID, type, &status);
	if (status == GL_FALSE)
	{
		std::vector<char> v(SHADER_ERROR_BUFFER_SIZE);
		glGetShaderInfoLog(shaderID, SHADER_ERROR_BUFFER_SIZE, nullptr, v.data());
		LOG_ERROR("{}\n{}", message, v.data());
	}
}

void ShaderProgram::CheckProgram(const std::string& message, GLenum type) const
{
	GLint status;
	glGetProgramiv(programID, type, &status);
	if (status == GL_FALSE)
	{
		std::vector<char> v(SHADER_ERROR_BUFFER_SIZE);
		glGetProgramInfoLog(programID, SHADER_ERROR_BUFFER_SIZE, nullptr, v.data());
		LOG_ERROR("{}\n{}", message, v.data());
	}
}

void ShaderProgram::Start() const
{
	glUseProgram(programID);
}

void ShaderProgram::Stop() const
{
	glUseProgram(0);
}

GLint ShaderProgram::GetUniformLocation(const char* name) const
{
	return glGetUniformLocation(programID, name);
}

void ShaderProgram::LoadUniform(GLint location, GLint value) const
{
	glUniform1i(location, value);
}

void ShaderProgram::LoadUniform(GLint location, GLuint value) const
{
	glUniform1ui(location, value);
}

void ShaderProgram::LoadUniform(GLint location, GLfloat value) const
{
	glUniform1f(location, value);
}

void ShaderProgram::LoadUniform(GLint location, bool value) const
{
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::LoadUniform(GLint location, const glm::vec3& vector) const
{
	glUniform3fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(GLint location, const glm::vec4& vector) const
{
	glUniform4fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(GLint location, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

// Memory management
ShaderProgram::~ShaderProgram()
{
	Stop();
	glDeleteProgram(programID);
}