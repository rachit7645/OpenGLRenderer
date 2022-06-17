#version 420 core

// TODO: Add specular mapping

const float AMBIENT_STRENGTH = 0.1f;
const float MIN_SPECULAR     = 0.0f;
const int   MAX_LIGHTS       = 4; 

struct Light
{
	vec4 position;
	vec4 colour;
	vec4 attenuation;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

in float visibility;
in vec2  pass_textureCoords;
in vec3  unitNormal;
in vec3  unitCameraVector;
in vec4  worldPosition;
in vec3  unitLightVector[MAX_LIGHTS];

uniform float     shineDamper;
uniform float     reflectivity;
uniform vec4      skyColour;
uniform sampler2D modelTexture;

out vec4 outColor;

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);
vec4  CalculateSpecular(int index);

void main()
{
	vec4 textureColor = texture(modelTexture, pass_textureCoords);
	// HACK: Alpha transparency check, should probably be in a separate shader
	if (textureColor.a < 0.5f)
		discard;

	vec4 totalDiffuse = vec4(0.0f);
	vec4 totalSpecular = vec4(0.0f);
	vec4 totalAmbient = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		float attFactor = CalculateAttFactor(i);
		totalDiffuse    += CalculateDiffuse(i)  * textureColor * attFactor;
		totalSpecular   += CalculateSpecular(i) * attFactor;
		totalAmbient    += CalculateAmbient(i)  * textureColor * attFactor;
	}

	// Add all lighting
	outColor = totalDiffuse + totalSpecular + totalAmbient;
	// Mix with fog colour
	outColor = mix(skyColour, outColor, visibility);
}

float CalculateAttFactor(int index)
{
	#define ATT lights[index].attenuation
	float distance = length(lights[index].position.xyz - worldPosition.xyz);
	float attFactor = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	return 1.0f / attFactor;
}

vec4 CalculateAmbient(int index)
{
	return vec4(AMBIENT_STRENGTH * lights[index].colour.xyz, 1.0f);
}

vec4 CalculateDiffuse(int index)
{
	float nDot1 = dot(unitNormal, unitLightVector[index]);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * lights[index].colour.xyz, 1.0f);
}

vec4 CalculateSpecular(int index)
{
	vec3 lightDirection = -unitLightVector[index];
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, 0.0f);
	float dampedFactor = pow(specularFactor, shineDamper);
	return vec4(dampedFactor * reflectivity * lights[index].colour.xyz, 1.0f);
}