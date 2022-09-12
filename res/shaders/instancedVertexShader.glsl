#version 430 core

const float FOG_DENSITY  = 0.0035f;
const float FOG_GRADIENT = 1.5f;
const int   MAX_LIGHTS   = 4;

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

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
};

layout(std430, binding = 3) buffer InstanceData
{
	Instance instances[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

out float visibility;
out vec2  txCoords;
out vec3  unitNormal;
out vec3  unitLightVector[MAX_LIGHTS];
out vec4  worldPosition;

void CalculateLighting();
void CalculateVisibility(vec4 posRelToCam);

void main()
{
	worldPosition      = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	vec4 posRelToCam   = viewMatrix * worldPosition;
	gl_Position        = projectionMatrix * posRelToCam;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	txCoords           = textureCoords;
	
	CalculateLighting();
	CalculateVisibility(posRelToCam);
}

void CalculateLighting()
{
	unitNormal = normalize((instances[gl_InstanceID].modelMatrix * vec4(normal, 0.0f)).xyz);
	
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		unitLightVector[i] = normalize(lights[i].position.xyz - worldPosition.xyz);
	}
}

void CalculateVisibility(vec4 posRelToCam)
{
	float distance = length(posRelToCam.xyz);
	visibility     = exp(-pow((distance * FOG_DENSITY), FOG_GRADIENT));
	visibility     = clamp(visibility, 0.0f, 1.0f);
}