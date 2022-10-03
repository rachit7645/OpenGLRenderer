#include "ShadowInstancedShader.h"

#include "Maths.h"

using namespace Shader;

using Entities::Light;

ShadowInstancedShader::ShadowInstancedShader()
	: ShaderProgram(SHADOW_INSTANCED_SHADER_VERTEX_PATH, SHADOW_INSTANCED_SHADER_FRAGMENT_PATH)
{
	GetUniformLocations();
}

void ShadowInstancedShader::GetUniformLocations()
{
	uniforms["lightView"] = GetUniformLocation("lightView");
	uniforms["lightProj"] = GetUniformLocation("lightProj");
}

void ShadowInstancedShader::LoadProjection(f32 nearPlane, f32 farPlane)
{
	LoadUniform(uniforms["lightProj"], Maths::CreateOrthoMatrix(nearPlane, farPlane));
}

void ShadowInstancedShader::LoadView(const Light& light)
{
	LoadUniform(uniforms["lightView"], Maths::CreateLookAtMatrix(light.position));
}