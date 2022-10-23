#version 430 core

const float AMBIENT_STRENGTH = 1.0f;
const int   MAX_LIGHTS       = 4;
const int   NUM_LIGHTS       = 4;

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
vec4  CalculateDiffuse(int index, vec3 normal, vec3 lightDir);

void main()
{
	vec4 worldPosition = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	gl_Position        = projectionMatrix * viewMatrix * worldPosition;
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	txCoords           = textureCoords;

	vec4 transNormal = instances[gl_InstanceID].modelMatrix * vec4(normal, 0.0f);
	vec3 unitNormal  = normalize(transNormal.xyz);

	for (int i = 0; i < min(MAX_LIGHTS, NUM_LIGHTS); ++i)
	{
		vec3  lightDir  = normalize(lights[i].position.xyz - worldPosition.xyz);
		float attFactor = CalculateAttFactor(i, worldPosition);
		vec4  ambient   = CalculateAmbient(i) * attFactor;
		vec4  diffuse   = CalculateDiffuse(i, unitNormal, lightDir);
		vertexColor    += ambient + diffuse;
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

vec4 CalculateDiffuse(int index, vec3 normal, vec3 lightDir)
{
	float nDot1      = dot(normal, lightDir);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * lights[index].diffuse.rgb, 1.0f);
}