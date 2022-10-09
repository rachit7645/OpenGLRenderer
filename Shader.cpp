#include "Shader.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <fmt/format.h>

#include "Util.h"
#include "Log.h"
#include "Files.h"

using namespace Shader;

constexpr auto SHADER_ERROR_BUFFER_SIZE = 4096;

ShaderProgram::ShaderProgram(const std::string_view vertexPath, const std::string_view fragmentPath)
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

void ShaderProgram::DumpToFile(const std::string_view path) const
{
	GLint length;
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length);

	GLenum format;
	auto binary = std::vector<char>(length);
	glGetProgramBinary
	(
		programID,
		static_cast<GLsizei>(binary.size()),
		&length,
		&format,
		reinterpret_cast<void*>(binary.data())
	);

	LOG_DEBUG("Dumping Shader to: {}\n", path);
	auto file = std::ofstream(path.data());
	file.write(binary.data(), length);
}

u32 ShaderProgram::LoadShader(GLenum type, const std::string_view path)
{
	LOG_INFO("Loading shader: {}\n", path);
	auto fs = std::ifstream(Files::GetResourceDirectory() + path.data(), std::ios::in);

	if (!fs.is_open())
	{
		LOG_ERROR("Unable to open shader: {}\n", path);
	}

	std::stringstream buffer;
	buffer << fs.rdbuf();
	std::string content = buffer.str();

	u32 shaderID = glCreateShader(type);
	const GLchar* cstr = content.c_str();
	glShaderSource(shaderID, 1, &cstr, nullptr);
	glCompileShader(shaderID);
	CheckShader(fmt::format("Shader Compilation Failed: {}", path), shaderID, GL_COMPILE_STATUS);

	return shaderID;
}

void ShaderProgram::CheckShader(const std::string_view message, u32 shaderID, GLenum type) const
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

void ShaderProgram::CheckProgram(const std::string_view message, GLenum type) const
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

ShaderProgram::~ShaderProgram()
{
	Stop();
	glDeleteProgram(programID);
}