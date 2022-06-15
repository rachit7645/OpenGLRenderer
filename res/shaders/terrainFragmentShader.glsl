#version 420 core

const float MIN_BRIGHTNESS = 0.2f;
const float MIN_SPECULAR   = 0.0f;
const float TEXTURE_TILING = 40.0f;
const int   MAX_LIGHTS     = 4; 

struct Light
{
	vec4 position;
	vec4 colour;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector[MAX_LIGHTS];
in vec3 toCameraVector;
in float visibility;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D bTexture;
uniform sampler2D gTexture;
uniform sampler2D blendMap;

uniform vec4 skyColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 outColor;

vec3 CalculateDiffuse(int index, vec3 unitNormal, vec3 unitLightVector);
vec3 CalculateSpecular(int index, vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector);
vec4 CalculateTextureColor();

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitCameraVector = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0f); vec3 totalSpecular = vec3(0.0f);
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		vec3 unitLightVector = normalize(toLightVector[i]);
		totalDiffuse += CalculateDiffuse(i, unitNormal, unitLightVector);
		totalSpecular += CalculateSpecular(i, unitNormal, unitLightVector, unitCameraVector);
	}

	totalDiffuse = max(totalDiffuse, vec3(MIN_BRIGHTNESS));
	vec4 textureColor = CalculateTextureColor();

	outColor = vec4(totalDiffuse, 1.0f) * textureColor + vec4(totalSpecular, 1.0f);
	outColor = mix(skyColour, outColor, visibility);
}

vec3 CalculateDiffuse(int index, vec3 unitNormal, vec3 unitLightVector)
{
	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, MIN_BRIGHTNESS);
	return brightness * lights[index].colour.xyz;
}

vec3 CalculateSpecular(int index, vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector)
{
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, MIN_SPECULAR);
	float dampedFactor = pow(specularFactor, shineDamper);
	return dampedFactor * reflectivity * lights[index].colour.xyz;
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