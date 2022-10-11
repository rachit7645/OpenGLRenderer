#version 430 core

const int MAX_LIGHTS = 4;

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

struct Instance
{
	mat4 modelMatrix;
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

layout (location = 0) in vec3 position;

void main()
{
	gl_Position = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
}