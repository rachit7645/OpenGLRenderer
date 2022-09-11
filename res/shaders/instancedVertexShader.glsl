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

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
};

layout(std430, binding = 3) buffer InstanceData
{
	mat4 modelMatrices[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

out vec2 txCoords;
out vec3 unitNormal;
out vec3 unitLightVector[MAX_LIGHTS];
out vec4 worldPosition;

void CalculateLighting();

void main()
{
	worldPosition      = modelMatrices[gl_InstanceID] * vec4(position, 1.0f);
	vec4 posRelToCam   = viewMatrix * worldPosition;
	gl_Position        = projectionMatrix * posRelToCam;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	txCoords           = textureCoords;
	
	CalculateLighting();
}

void CalculateLighting()
{
	unitNormal = normalize((modelMatrices[gl_InstanceID] * vec4(normal, 0.0f)).xyz);
	
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		unitLightVector[i] = normalize(lights[i].position.xyz - worldPosition.xyz);
	}
}