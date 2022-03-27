#include "Shader.h"

using namespace Shader;

// Creates a vertex and fragment shader and links em' into a program
ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
{
	programID = glCreateProgram();
	vertexShaderID = LoadShader(GL_VERTEX_SHADER, vertexPath);
	fragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragmentPath);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
}

void ShaderProgram::BindAttribute(u32 attribute, const char* name) const
{
	glBindAttribLocation(programID, attribute, name);
}

u32 ShaderProgram::GetUniformLocation(const char* name) const
{
	return glGetUniformLocation(programID, name);
}

void ShaderProgram::LoadInt(u32 location, u32 value) const
{
	glUniform1i(location, value);
}

void ShaderProgram::LoadFloat(u32 location, f32 value) const
{
	glUniform1f(location, value);
}

void ShaderProgram::LoadBool(u32 location, bool value) const
{
	glUniform1f(location, value ? 1 : 0);
}

void ShaderProgram::LoadVector(u32 location, const glm::vec3& vector) const
{
	glUniform3fv(location, 1, &vector[0]);
}

void ShaderProgram::LoadMatrix(u32 location, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

// Util function to load shaders
u32 ShaderProgram::LoadShader(GLenum type, const std::string& path)
{
	std::string content;
	#ifdef _DEBUG
    	std::ifstream fileStream("../" + path, std::ios::in);
	#else
		std::ifstream fileStream(path, std::ios::in);
	#endif
    if (!fileStream.is_open())
	{
       Logger::LogAndExit("Unable to open file" + path, SHADER_FILE_OPEN_FAILED);
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
		Logger::LogAndExit(v.data(), SHADER_COMPILATION_FAILED);
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

// Memory management
ShaderProgram::~ShaderProgram()
{
	Stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}