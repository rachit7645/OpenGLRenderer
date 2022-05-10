#version 330 core

const float MIN_BRIGHTNESS = 0.15f;
const float MIN_SPECULAR   = 0.0f;
const float TEXTURE_TILING = 40.0f;

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in float visibility;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D bTexture;
uniform sampler2D gTexture;
uniform sampler2D blendMap;

uniform vec4 skyColour;
uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 outColor;

vec3 CalculateDiffuse(vec3 unitNormal, vec3 unitLightVector);
vec3 CalculateSpecular(vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector);
vec4 CalculateTextureColor();

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	vec3 unitCameraVector = normalize(toCameraVector);

	vec3 diffuse = CalculateDiffuse(unitNormal, unitLightVector);
	vec3 finalSpecular = CalculateSpecular(unitNormal, unitLightVector, unitCameraVector);
	vec4 textureColor = CalculateTextureColor();

	outColor = vec4(diffuse, 1.0f) * textureColor + vec4(finalSpecular, 1.0f);
	outColor = mix(skyColour, outColor, visibility);
}

vec3 CalculateDiffuse(vec3 unitNormal, vec3 unitLightVector)
{
	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, MIN_BRIGHTNESS);
	return brightness * lightColour;
}

vec3 CalculateSpecular(vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector)
{
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, MIN_SPECULAR);
	float dampedFactor = pow(specularFactor, shineDamper);
	return dampedFactor * reflectivity * lightColour;
}

vec4 CalculateTextureColor()
{
	vec4 blendMapColor = texture(blendMap, pass_textureCoords);
    float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
    vec2 tiledCoords = pass_textureCoords * 40.0;
    vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
    vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
    vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
    vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;
	return backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
}