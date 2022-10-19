#version 430 core

const float AMBIENT_STRENGTH = 0.2f;
const float MIN_SPECULAR     = 0.0f;
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

layout(std140, binding = 2) uniform Shared
{
	vec4  clipPlane;
	vec4  skyColor;
	vec4  cameraPos;
	float farPlane;
};

in vec2 txCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

out vec4 outColor;

float CalculateAttFactor(int index, vec3 worldPosition);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index, vec3 unitNormal, vec3 unitLightVector);

void main()
{
	// Retrieve data from G-buffer
	vec3  fragPos    = texture(gPosition,   txCoords).rgb;
	vec3  normal     = texture(gNormal,     txCoords).rgb;
	vec4  albedoSpec = texture(gAlbedoSpec, txCoords);
	vec3  albedo     = albedoSpec.rgb;
	float spec       = albedoSpec.a;

	vec4 ambient  = vec4(0.0f);
	vec4 diffuse  = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		// Light direction
		vec3 lightDir = normalize(lights[i].position.xyz - fragPos);
		// Light attenuation factor
		float attFactor = CalculateAttFactor(i, fragPos);
		// Light ambient factor
		ambient += CalculateAmbient(i) * vec4(albedo, 1.0f) * attFactor;
		// Light diffuse factor
		diffuse += CalculateDiffuse(i, normal, lightDir) * vec4(albedo, 1.0f) * attFactor;
	}

	outColor = ambient + diffuse;
}

float CalculateAttFactor(int index, vec3 worldPosition)
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

vec4 CalculateDiffuse(int index, vec3 unitNormal, vec3 unitLightVector)
{
	float nDot1      = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * lights[index].diffuse.rgb, 1.0f);
}