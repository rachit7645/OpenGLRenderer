#include "Shader.h"

using namespace Shader;

ShaderProgram Shader::CreateShader(std::string vertexPath, std::string fragmentPath)
{
	ShaderProgram m_shader;
	m_shader.programID = glCreateProgram();
	m_shader.vertexShaderID = LoadShader(GL_VERTEX_SHADER, vertexPath);
	m_shader.fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	glAttachShader(m_shader.programID, m_shader.vertexShaderID);
	glAttachShader(m_shader.programID, m_shader.fragmentShaderID);
	glLinkProgram(m_shader.programID);
	glValidateProgram(m_shader.programID);

	return m_shader;
}

// Util function to load shaders
u32 LoadShader(GLenum type, std::string path)
{
	std::string content;
	#ifdef _DEBUG
    	std::ifstream fileStream("../" + path, std::ios::in);
	#else
		std::ifstream fileStream(path, std::ios::in);
	#endif
    if (!fileStream.is_open())
	{
       Logger::LogAndExit("Unable to open file" + path, 5);
    }

    std::string line;
    while (std::getline(fileStream, line)) content.append(line + "\n");
	fileStream.close();
	
	u32 shaderID = glCreateShader(type);
	const GLchar* cstr = content.c_str();
	glShaderSource(shaderID, 1, &cstr, NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		constexpr GLint length = 200;
		std::vector<char> v;
		v.reserve(length);
		glGetShaderInfoLog(shaderID, length, NULL, v.data());
		Logger::LogAndExit(v.data(), 6);
	}

	return shaderID;
}

void ShaderProgram::Start()
{
	glUseProgram(this->programID);
}

void ShaderProgram::Stop()
{
	glUseProgram(0);
}

void ShaderProgram::CleanUP()
{
	Stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}