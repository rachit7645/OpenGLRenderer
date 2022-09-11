#version 430 core

const int MAX_LIGHTS   = 4;

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
};

layout(std430, binding = 4) buffer InstanceData
{
	mat4 modelMatrices[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

out vec2 txCoords;

void main()
{
	vec4 worldPosition = modelMatrices[gl_InstanceID] * vec4(position, 1.0f);
	vec4 posRelToCam   = viewMatrix * worldPosition;
	gl_Position        = projectionMatrix * posRelToCam;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
}