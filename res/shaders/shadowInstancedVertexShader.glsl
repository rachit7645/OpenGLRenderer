#version 430 core

struct Instance
{
	mat4 modelMatrix;
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 lightProj;
	mat4 lightView;
};

layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

layout (location = 0) in vec3 position;

void main()
{
	gl_Position = lightProj * lightView * instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
}