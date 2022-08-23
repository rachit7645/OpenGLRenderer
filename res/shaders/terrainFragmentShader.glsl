#version 430 core

const float AMBIENT_STRENGTH = 0.2f;
const float TEXTURE_TILING   = 40.0f;
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
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
	vec2 frustum;
};

in vec4  worldPosition;
in vec3  unitNormal;
in vec3  unitLightVector[MAX_LIGHTS];
in vec2  txCoords;
in float visibility;

uniform sampler2D bgTexture;
uniform sampler2D rTexture;
uniform sampler2D bTexture;
uniform sampler2D gTexture;
uniform sampler2D blendMap;

out vec4 outColor;

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);
vec4  CalculateTextureColor();

void main()
{
	vec4 textureColor = CalculateTextureColor();

	vec4 totalAmbient  = vec4(0.0f);
	vec4 totalDiffuse  = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		float attFactor = CalculateAttFactor(i);
		totalAmbient += CalculateAmbient(i) * textureColor * attFactor;
		totalDiffuse += CalculateDiffuse(i) * textureColor * attFactor;
	}

	// Add all lighting
	outColor = totalDiffuse + totalAmbient;
	// Mix with fog colour
	outColor = mix(skyColor, outColor, visibility);
}

float CalculateAttFactor(int index)
{
	#define ATT lights[index].attenuation
	float dist      = length(lights[index].position.xyz - worldPosition.xyz);
	float attFactor = ATT.x + (ATT.y * dist) + (ATT.z * dist * dist);
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

vec4 CalculateTextureColor()
{
	vec4  blendMapColor   = texture(blendMap, txCoords);
    float backTxAmount    = 1.0f - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
    vec2  tiledCoords     = txCoords * TEXTURE_TILING;
    vec4  bgTextureColor  = texture(bgTexture, tiledCoords) * backTxAmount;
    vec4  rTextureColor   = texture(rTexture, tiledCoords)  * blendMapColor.r;
    vec4  gTextureColor   = texture(gTexture, tiledCoords)  * blendMapColor.g;
    vec4  bTextureColor   = texture(bTexture, tiledCoords)  * blendMapColor.b;
	return bgTextureColor + rTextureColor + gTextureColor + bTextureColor;
}