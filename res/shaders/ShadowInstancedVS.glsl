#version 430 core

// Struct describing an instance of an entity
struct Instance
{
	mat4 modelMatrix;
};

// Instances data SSBO
layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

// Vertex inputs
layout (location = 0) in vec3 position;

// Entry point
void main()
{
	// Transform to world space
	gl_Position = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
}