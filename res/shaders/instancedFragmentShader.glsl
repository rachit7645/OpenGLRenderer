#version 430 core

const float AMBIENT_STRENGTH = 0.2f;
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
uniform sampler2D specularTexture;

out vec4 outColor;

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);

void main()
{
	vec4 diffuseColor = texture(diffuseTexture, txCoords);
	
	if (diffuseColor.a < 0.5f)
	{
		discard;
	}
	
	vec4 totalAmbient = vec4(0.0f);
	vec4 totalDiffuse = vec4(0.0f);
	
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		// Compute Attenuation
		float attFactor = CalculateAttFactor(i);
		// Compute ambient
		totalAmbient += CalculateAmbient(i) * diffuseColor * attFactor;
		// Compute diffuse
		totalDiffuse += CalculateDiffuse(i) * diffuseColor * attFactor;
	}

	// Add all lighting
	outColor = totalDiffuse + totalAmbient;
}

float CalculateAttFactor(int index)
{
	#define ATT lights[index].attenuation
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