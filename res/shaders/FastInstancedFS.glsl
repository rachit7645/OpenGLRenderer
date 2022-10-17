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
in vec4 worldPosition;

uniform sampler2D diffuseTexture;

out vec4 outColor;

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);

void main()
{
	// Diffuse texture color
	vec4 diffuseColor = texture(diffuseTexture,  txCoords);
	// Lighting loop
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		// Compute ambient
		outColor += CalculateAmbient(i) * diffuseColor * CalculateAttFactor(i);
	}
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