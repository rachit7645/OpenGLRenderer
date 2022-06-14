#version 420 core

const float MIN_BRIGHTNESS = 0.15f;
const float MIN_SPECULAR   = 0.0f;

layout(std140, binding = 1) uniform Lights
{
	uniform vec4 lightPosition;
	uniform vec4 lightColour;
};

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in float visibility;

uniform sampler2D modelTexture;
uniform vec4 skyColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 outColor;

vec3 CalculateDiffuse(vec3 unitNormal, vec3 unitLightVector);
vec3 CalculateSpecular(vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector);

void main()
{
	vec4 textureColour = texture(modelTexture, pass_textureCoords);
	if (textureColour.a < 0.5f)
		discard;

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	vec3 unitCameraVector = normalize(toCameraVector);

	vec3 diffuse = CalculateDiffuse(unitNormal, unitLightVector);
	vec3 finalSpecular = CalculateSpecular(unitNormal, unitLightVector, unitCameraVector);

	outColor = vec4(diffuse, 1.0f) * textureColour + vec4(finalSpecular, 1.0f);
	outColor = mix(skyColour, outColor, visibility);
}

vec3 CalculateDiffuse(vec3 unitNormal, vec3 unitLightVector)
{
	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, MIN_BRIGHTNESS);
	return brightness * lightColour.xyz;
}

vec3 CalculateSpecular(vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector)
{
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, MIN_SPECULAR);
	float dampedFactor = pow(specularFactor, shineDamper);
	return dampedFactor * reflectivity * lightColour.xyz;
}