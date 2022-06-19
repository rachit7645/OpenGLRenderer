#include "Shader.h"

using namespace Shader;

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
	programID = glCreateProgram();
	u32 vertexShaderID = LoadShader(GL_VERTEX_SHADER, vertexPath);
	u32 fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	CheckProgram("Shader link failed for: " + vertexPath + ", " + fragmentPath, programID, GL_LINK_STATUS);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glValidateProgram(programID);
	CheckProgram("Shader validation failed for: " + vertexPath + ", " + fragmentPath, programID, GL_VALIDATE_STATUS);
}

void ShaderProgram::BindAttribute(u32 attribute, const char* name)
{
	glBindAttribLocation(programID, attribute, name);
}

u32 ShaderProgram::GetUniformLocation(const char* name)
{
	return glGetUniformLocation(programID, name);
}

void ShaderProgram::LoadUniform(u32 location, GLint value)
{
	glUniform1i(location, value);
}

void ShaderProgram::LoadUniform(u32 location, GLuint value)
{
	glUniform1ui(location, value);
}

void ShaderProgram::LoadUniform(u32 location, GLfloat value)
{
	glUniform1f(location, value);
}

void ShaderProgram::LoadUniform(u32 location, bool value)
{
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::LoadUniform(u32 location, const glm::vec3& vector)
{
	glUniform3fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(u32 location, const glm::vec4& vector)
{
	glUniform4fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadUniform(u32 location, const glm::mat4& matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::DumpToFile(const std::string& path)
{
	GLint length;
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length);

	GLenum format;
	std::vector<char> binary(length);
	glGetProgramBinary(programID, binary.size(), &length, &format, reinterpret_cast<void*>(binary.data()));

	std::ofstream file(path);
	file.write(binary.data(), length);

	LOG_DEBUG("Dumping Shader to: ", path, "\n");
}

u32 ShaderProgram::LoadShader(GLenum type, const std::string& path)
{
	LOG_INFO("Loading shader: ", path, "\n");
	std::string newPath = Files::GetResourceDirectory() + path;
	std::ifstream fileStream(newPath, std::ios::in);

	if (!fileStream.is_open())
	{
		LOG_ERROR("Unable to open shader: ", path);
	}
	std::string content = std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

	u32 shaderID = glCreateShader(type);
	const GLchar* cstr = content.c_str();
	glShaderSource(shaderID, 1, &cstr, nullptr);
	glCompileShader(shaderID);
	CheckShader("Shader Compilation Failed: " + path, shaderID, GL_COMPILE_STATUS);

	return shaderID;
}

void ShaderProgram::CheckShader(const std::string& message, u32 shaderID, GLenum type)
{
	GLint status;
	glGetShaderiv(shaderID, type, &status);
	if (status == GL_FALSE)
	{
		std::vector<char> v(SHADER_ERROR_BUFFER_SIZE);
		glGetShaderInfoLog(shaderID, SHADER_ERROR_BUFFER_SIZE, nullptr, v.data());
		LOG_ERROR(message, "\n", v.data());
	}
}

void ShaderProgram::CheckProgram(const std::string& message, u32 programID, GLenum type)
{
	GLint status;
	glGetProgramiv(programID, type, &status);
	if (status == GL_FALSE)
	{
		std::vector<char> v(SHADER_ERROR_BUFFER_SIZE);
		glGetProgramInfoLog(programID, SHADER_ERROR_BUFFER_SIZE, nullptr, v.data());
		LOG_ERROR(message, "\n", v.data());
	}
}

void ShaderProgram::Start()
{
	glUseProgram(programID);
}

void ShaderProgram::Stop()
{
	glUseProgram(0);
}

// Memory management
ShaderProgram::~ShaderProgram()
{
	Stop();
	glDeleteProgram(programID);
}