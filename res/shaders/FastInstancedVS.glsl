#version 430 core

const float AMBIENT_STRENGTH = 1.0f;
const int   MAX_LIGHTS       = 4;

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
out vec4 vertexColor;

float CalculateAttFactor(int index, vec4 worldPosition);
vec4  CalculateAmbient(int index);

void main()
{
	vec4 worldPosition = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	gl_Position        = projectionMatrix * viewMatrix * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	txCoords           = textureCoords;

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		vertexColor += CalculateAmbient(i) * CalculateAttFactor(i, worldPosition);
	}
}

float CalculateAttFactor(int index, vec4 worldPosition)
{
	vec4  ATT       = lights[index].attenuation;
	float distance  = length(lights[index].position.xyz - worldPosition.xyz);
	float attFactor = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	return 1.0f / attFactor;
}

vec4 CalculateAmbient(int index)
{
	return vec4(AMBIENT_STRENGTH * lights[index].ambient.rgb, 1.0f);
}