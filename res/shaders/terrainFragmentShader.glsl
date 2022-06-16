#version 420 core

const float AMBIENT_STRENGTH = 0.1f;
const float MIN_SPECULAR     = 0.0f;
const float TEXTURE_TILING   = 40.0f;
const int   MAX_LIGHTS       = 4; 

struct Light
{
	vec4 position;
	vec4 colour;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

in float visibility;
in vec2  pass_textureCoords;
in vec3  unitNormal;
in vec3  unitCameraVector;
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

vec3 CalculateAmbient(int index);
vec3 CalculateDiffuse(int index);
vec3 CalculateSpecular(int index);
vec4 CalculateTextureColor();

void main()
{
	vec4 textureColor = CalculateTextureColor();

	vec3 totalDiffuse = vec3(0.0f);
	vec3 totalSpecular = vec3(0.0f);
	vec3 totalAmbient = vec3(0.0f);

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		totalDiffuse += CalculateDiffuse(i);
		totalSpecular += CalculateSpecular(i);
		totalAmbient += CalculateAmbient(i);
	}

	// Add all lighting
	outColor = vec4(totalDiffuse, 1.0f) + vec4(totalSpecular, 1.0f) + vec4(totalAmbient, 1.0f);
	// Multiply by texture colour
	outColor *= textureColor;
	// Mix with fog colour
	outColor = mix(skyColour, outColor, visibility);
}

vec3 CalculateAmbient(int index)
{
	return AMBIENT_STRENGTH * lights[index].colour.xyz;
}

vec3 CalculateDiffuse(int index)
{
	float nDot1 = dot(unitNormal, unitLightVector[index]);
	float brightness = max(nDot1, 0.0f);
	return brightness * lights[index].colour.xyz;
}

vec3 CalculateSpecular(int index)
{
	vec3 lightDirection = -unitLightVector[index];
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