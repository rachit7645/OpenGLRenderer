#version 430 core

const int MAX_LIGHTS = 4;

struct Light
{
	vec4 position;
	vec4 color;
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
	int   numLights;
	Light lights[MAX_LIGHTS];
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
out vec3 unitLightVector[MAX_LIGHTS];
out vec4 worldPosition;

void main()
{
	worldPosition      = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	gl_Position        = projectionMatrix * viewMatrix * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	txCoords           = textureCoords;

	vec4 transNormal = instances[gl_InstanceID].modelMatrix * vec4(normal, 0.0f);
	unitNormal       = normalize(transNormal.xyz);

	for (int i = 0; i < numLights; ++i)
	{
		unitLightVector[i] = normalize(lights[i].position.xyz - worldPosition.xyz);
	}
}