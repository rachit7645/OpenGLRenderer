#include "Shader.h"

using namespace Shader;

ShaderProgram::ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath)
{
	programID = glCreateProgram();
	u32 vertexShaderID = LoadShader(GL_VERTEX_SHADER, vertexPath);
	u32 fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	CheckProgram(programID, GL_LINK_STATUS, SHADER_LINK_FAILED);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glValidateProgram(programID);
	CheckProgram(programID, GL_VALIDATE_STATUS, SHADER_VALIDATION_FAILED);
}

void ShaderProgram::BindAttribute(u32 attribute, const char *name) const
{
	glBindAttribLocation(programID, attribute, name);
}

u32 ShaderProgram::GetUniformLocation(const char *name) const
{
	return glGetUniformLocation(programID, name);
}

void ShaderProgram::LoadInt(u32 location, s32 value) const
{
	glUniform1i(location, value);
}

void ShaderProgram::LoadFloat(u32 location, f32 value) const
{
	glUniform1f(location, value);
}

void ShaderProgram::LoadBool(u32 location, bool value) const
{
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::LoadVector(u32 location, const glm::vec3 &vector) const
{
	glUniform3fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadVector(u32 location, const glm::vec4& vector) const
{
	glUniform4fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadMatrix(u32 location, const glm::mat4 &matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

u32 ShaderProgram::LoadShader(GLenum type, const std::string &path)
{
	#ifdef _DEBUG
		std::ifstream fileStream("../" + path, std::ios::in);
	#else
		std::ifstream fileStream(path, std::ios::in);
	#endif
	if (!fileStream.is_open())
	{
		Logger::LogAndExit("Unable to open file" + path, SHADER_FILE_OPEN_FAILED);
	}
	std::string content = std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

	u32 shaderID = glCreateShader(type);
	const GLchar *cstr = content.c_str();
	glShaderSource(shaderID, 1, &cstr, nullptr);
	glCompileShader(shaderID);
	CheckShader("Shader Compilation Failed: " + path + "\n", shaderID, GL_COMPILE_STATUS, SHADER_COMPILATION_FAILED);

	return shaderID;
}

void ShaderProgram::CheckShader(const std::string &message, u32 shaderID, GLenum type, Error error)
{
	GLint status;
	glGetShaderiv(shaderID, type, &status);
	if (status == GL_FALSE)
	{
		std::vector<char> v(512);
		glGetShaderInfoLog(shaderID, 512, nullptr, v.data());
		Logger::LogAndExit(message + v.data(), error);
	}
}

void ShaderProgram::CheckProgram(u32 programID, GLenum type, Error error)
{
	GLint status;
	glGetProgramiv(programID, type, &status);
	if (status == GL_FALSE)
	{
		std::vector<char> v(512);
		glGetProgramInfoLog(programID, 512, nullptr, v.data());
		Logger::LogAndExit(v.data(), error);
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