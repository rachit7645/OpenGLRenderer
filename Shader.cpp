#include "Shader.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <fmt/format.h>

#include "Util.h"
#include "Log.h"
#include "Files.h"

// Using namespaces
using namespace Shader;

// Usings
using Engine::Files;

// Constants
constexpr auto SHADER_ERROR_BUFFER_SIZE = 4096;

ShaderProgram::ShaderProgram(const std::string_view vertexPath, const std::string_view fragmentPath)
{
	// Create shader program
	programID = glCreateProgram();

	// Create shaders
	GLuint vertexShaderID   = LoadShader(GL_VERTEX_SHADER,   vertexPath);
	GLuint fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	// Attach shaders
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	// Link program
	glLinkProgram(programID);
	CheckProgram(fmt::format("Shader link failed for: {}, {}", vertexPath, fragmentPath), GL_LINK_STATUS);

	// Clear shaders
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	// Validate program
	glValidateProgram(programID);
	CheckProgram(fmt::format("Shader validation failed for: {}, {}", vertexPath, fragmentPath), GL_VALIDATE_STATUS);

	// Get file handler instance
	auto& files = Files::GetInstance();
	// Calculate dump name
	auto vertex   = files.GetName(vertexPath);
	auto fragment = files.GetName(fragmentPath);
	// Dump shader
	DumpToFile("dumps/" + vertex + "_" + fragment);
}

ShaderProgram::ShaderProgram
(
	const std::string_view vertexPath,
	const std::string_view fragmentPath,
	const std::string_view geometryPath
)
{
	// Create shader program
	programID = glCreateProgram();

	// Create shaders
	GLuint vertexShaderID   = LoadShader(GL_VERTEX_SHADER,   vertexPath);
	GLuint fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);
	GLuint geometryShaderID = LoadShader(GL_GEOMETRY_SHADER, geometryPath);

	// Attach shaders
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glAttachShader(programID, geometryShaderID);

	// Link program
	glLinkProgram(programID);
	CheckProgram(fmt::format("Shader link failed for: {}, {}, {}", vertexPath, fragmentPath, geometryPath), GL_LINK_STATUS);

	// Clear shaders
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(geometryShaderID);

	// Validate program
	glValidateProgram(programID);
	CheckProgram(fmt::format("Shader validation failed for: {}, {}, {}", vertexPath, fragmentPath, geometryPath), GL_VALIDATE_STATUS);

	// Get file handler instance
	auto& files = Files::GetInstance();
	// Calculate dump name
	auto vertex   = files.GetName(vertexPath);
	auto fragment = files.GetName(fragmentPath);
	auto geometry = files.GetName(geometryPath);
	// Dump shader
	DumpToFile("dumps/" + vertex + "_" + fragment + "_" + geometry);
}

void ShaderProgram::DumpToFile(const std::string_view path) const
{
	// Query shader binary size
	GLint length;
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length);

	// Allocate space for dump
	auto binary = std::vector<char>(length);

	// Get shader dump
	GLenum format;
	glGetProgramBinary
	(
		programID,
		static_cast<GLsizei>(binary.size()),
		&length,
		&format,
		reinterpret_cast<void*>(binary.data())
	);

	// Write to file
	auto file = std::ofstream(path.data());
	file.write(binary.data(), length);
}

GLuint ShaderProgram::LoadShader(GLenum type, const std::string_view path)
{
	// Log
	LOG_INFO("Loading shader: {}\n", path);
	// Get file handler
	auto& files = Files::GetInstance();
	// Load file
	auto fs = std::ifstream(files.GetResources() + path.data(), std::ios::in);

	// If file load failed
	if (!fs.is_open())
	{
		// Log error
		LOG_ERROR("Unable to open shader: {}\n", path);
	}

	// Get file data
	std::stringstream buffer;
	buffer << fs.rdbuf();
	// Copy to string
	std::string content = buffer.str();

	// Create shader
	GLuint shaderID = glCreateShader(type);
	// Load shader code
	const GLchar* cstr = content.c_str();
	glShaderSource(shaderID, 1, &cstr, nullptr);
	// Compile
	glCompileShader(shaderID);
	// Check for errors
	CheckShader(fmt::format("Shader Compilation Failed: {}", path), shaderID, GL_COMPILE_STATUS);

	// Return
	return shaderID;
}

void ShaderProgram::CheckShader(const std::string_view message, u32 shaderID, GLenum type) const
{
	// Query status
	GLint status;
	glGetShaderiv(shaderID, type, &status);

	// If compilation failed
	if (status == GL_FALSE)
	{
		// Allocate error buffer
		auto error = std::vector<char>(SHADER_ERROR_BUFFER_SIZE);
		// Get error
		glGetShaderInfoLog(shaderID, SHADER_ERROR_BUFFER_SIZE, nullptr, error.data());
		// Log error
		LOG_ERROR("{}\n{}", message, error.data());
	}
}

void ShaderProgram::CheckProgram(const std::string_view message, GLenum type) const
{
	// Query program status
	GLint status;
	glGetProgramiv(programID, type, &status);

	// If linking failed
	if (status == GL_FALSE)
	{
		// Allocate error buffer
		auto error = std::vector<char>(SHADER_ERROR_BUFFER_SIZE);
		// Get error
		glGetProgramInfoLog(programID, SHADER_ERROR_BUFFER_SIZE, nullptr, error.data());
		// Log error
		LOG_ERROR("{}\n{}", message, error.data());
	}
}

void ShaderProgram::Start() const
{
	// Use current shader
	glUseProgram(programID);
}

void ShaderProgram::Stop() const
{
	// Unbind shader
	glUseProgram(0);
}

GLint ShaderProgram::GetUniformLocation(const char* name) const
{
	// Query location
	auto location = glGetUniformLocation(programID, name);

	// If location is invalid
	if (location == -1)
	{
		// Log warning
		LOG_WARNING("Shader Program {} : Uniform \"{}\" is invalid!\n", programID, name);
	}

	// Return location
	return location;
}

void ShaderProgram::LoadUniform(GLint location, GLint value) const
{
	// Load s32
	glUniform1i(location, value);
}

void ShaderProgram::LoadUniform(GLint location, GLuint value) const
{
	// Load u32
	glUniform1ui(location, value);
}

void ShaderProgram::LoadUniform(GLint location, GLfloat value) const
{
	// Load f32
	glUniform1f(location, value);
}

void ShaderProgram::LoadUniform(GLint location, bool value) const
{
	// Load s32 for bool
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::LoadUniform(GLint location, const glm::vec2& vector) const
{
	// Load vec2
	glUniform2fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(GLint location, const glm::vec3& vector) const
{
	// Load vec3
	glUniform3fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(GLint location, const glm::vec4& vector) const
{
	// Load vec4
	glUniform4fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(GLint location, const glm::mat4& matrix) const
{
	// Load mat4
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

ShaderProgram::~ShaderProgram()
{
	// ID must not be zero
	if (programID != 0)
	{
		// Stop shader, if running
		Stop();
		// Clear shader
		glDeleteProgram(programID);
	}
}