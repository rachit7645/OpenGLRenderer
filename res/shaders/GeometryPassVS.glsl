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
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
	vec4 resolution;
};

layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

out vec2 txCoords;
out vec3 unitNormal;
out vec3 worldPosition;

void main()
{
	vec4 fragPos       = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	worldPosition      = fragPos.xyz;
	vec4 posRelToCam   = viewMatrix * fragPos;
	gl_Position        = projectionMatrix * posRelToCam;
	txCoords           = textureCoords;
	vec4 transNormal   = instances[gl_InstanceID].modelMatrix * vec4(normal, 0.0f);
	unitNormal         = normalize(transNormal.xyz);
}