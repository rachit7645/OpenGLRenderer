#version 420 core

// TODO: Add specular mapping

const float AMBIENT_STRENGTH = 0.1f;
const float MIN_SPECULAR     = 0.0f;
const float TEXTURE_TILING   = 40.0f;
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

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D bTexture;
uniform sampler2D gTexture;
uniform sampler2D blendMap;

uniform float shineDamper;
uniform float reflectivity;
uniform vec4  skyColour;

out vec4 outColor;

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);
vec4  CalculateSpecular(int index);
vec4  CalculateTextureColor();

void main()
{
	vec4 textureColor = CalculateTextureColor();

	vec4 totalDiffuse = vec4(0.0f);
	vec4 totalSpecular = vec4(0.0f);
	vec4 totalAmbient = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		float attFactor = CalculateAttFactor(i);
		totalDiffuse  += CalculateDiffuse(i)  * textureColor * attFactor;
		totalSpecular += CalculateSpecular(i) * attFactor;
		totalAmbient  += CalculateAmbient(i)  * textureColor * attFactor;
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
	specularFactor = max(specularFactor, MIN_SPECULAR);
	float dampedFactor = pow(specularFactor, shineDamper);
	return vec4(dampedFactor * reflectivity * lights[index].colour.xyz, 1.0f);
}

vec4 CalculateTextureColor()
{
	vec4 blendMapColor = texture(blendMap, pass_textureCoords);
    float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
    vec2 tiledCoords = pass_textureCoords * TEXTURE_TILING;
    vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
    vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
    vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
    vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;
	return backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
}