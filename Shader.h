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
        // Shader with vertex and fragment stages
        ShaderProgram(const std::string_view vertexPath, const std::string_view fragmentPath);

        // Shader with vertex, geometry and fragment stages
        ShaderProgram
        (
            const std::string_view vertexPath,
            const std::string_view fragmentPath,
            const std::string_view geometryPath
        );

        // Destructor
        virtual ~ShaderProgram();

        // Start shader
        void Start() const;
        // Stop shader
        void Stop() const;

        // Shader program ID
        GLuint programID;
    protected:
        // Query uniform location
        GLint GetUniformLocation(const char* name) const;

        // Load s32 uniform
        void LoadUniform(GLint location, GLint value) const;
        // Load u32 uniform
        void LoadUniform(GLint location, GLuint value) const;
        // Load f32 uniform
        void LoadUniform(GLint location, GLfloat value) const;
        // Load bool uniform
        void LoadUniform(GLint location, bool value) const;
        // Load vec2 uniform
        void LoadUniform(GLint location, const glm::vec2& vector) const;
        // Load vec3 uniform
        void LoadUniform(GLint location, const glm::vec3& vector) const;
        // Load vec4 uniform
        void LoadUniform(GLint location, const glm::vec4& vector) const;
        // Load mat4 uniform
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