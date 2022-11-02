#version 430 core

const float AMBIENT_STRENGTH = 0.6f;
const int   MAX_LIGHTS       = 4;

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

in vec2 txCoords;
in vec3 unitNormal;
in vec3 unitLightVector[MAX_LIGHTS];
in vec4 worldPosition;

uniform sampler2D diffuseTexture;

out vec4 outColor;

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);

void main()
{
	vec4 diffColor = texture(diffuseTexture, txCoords);

	vec4 ambient = vec4(0.0f);
	vec4 diffuse = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		float attFactor = CalculateAttFactor(i);
		ambient        += CalculateAmbient(i) * diffColor * attFactor;
		diffuse        += CalculateDiffuse(i) * diffColor * attFactor;
	}

	outColor = ambient + diffuse;
}

float CalculateAttFactor(int index)
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

vec4 CalculateDiffuse(int index)
{
	float nDot1      = dot(unitNormal, unitLightVector[index]);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * lights[index].diffuse.rgb, 1.0f);
}