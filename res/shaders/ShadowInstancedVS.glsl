#version 430 core

struct Instance
{
	mat4 modelMatrix;
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