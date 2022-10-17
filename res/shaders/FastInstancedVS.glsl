#version 430 core

struct Instance
{
	mat4 modelMatrix;
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 2) uniform Shared
{
	vec4  clipPlane;
	vec4  skyColor;
	vec4  cameraPos;
	float farPlane;
};

layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

out vec2 txCoords;
out vec4 worldPosition;

void main()
{
	worldPosition      = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	gl_Position        = projectionMatrix * viewMatrix * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	txCoords           = textureCoords;
}