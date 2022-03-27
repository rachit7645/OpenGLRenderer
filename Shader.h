#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Util.h"
#include "Log.h"

namespace Shader
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		u32 programID;
		u32 vertexShaderID;
		u32 fragmentShaderID;

		void Start();
		void Stop();

		void BindAttribute(u32 attribute, const char* name) const;
		u32 GetUniformLocation(const char* name) const;
		virtual void BindAttributes() = 0;
		virtual void GetUniformLocations() = 0;

		void LoadInt(u32 location, u32 value) const;
		void LoadFloat(u32 location, f32 value) const;
		void LoadBool(u32 location, bool value) const;
		void LoadVector(u32 location, const glm::vec3& vector) const;
		void LoadMatrix(u32 location, const glm::mat4& matrix) const;

	private:
		u32 LoadShader(GLenum type, const std::string& path);
	};
}

#endif // SHADER_H