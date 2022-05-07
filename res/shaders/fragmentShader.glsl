#version 330 core

#define MAX_BRIGHTNESS	0.15f
#define MAX_SPECULAR	0.0f

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform sampler2D modelTexture;

uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 out_color;

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

	out_color = vec4(diffuse, 1.0f) * textureColour + vec4(finalSpecular, 1.0f);
}

vec3 CalculateDiffuse(vec3 unitNormal, vec3 unitLightVector)
{
	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, MAX_BRIGHTNESS);
	return brightness * lightColour;
}

vec3 CalculateSpecular(vec3 unitNormal, vec3 unitLightVector, vec3 unitCameraVector)
{
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, MAX_SPECULAR);
	float dampedFactor = pow(specularFactor, shineDamper);
	return dampedFactor * reflectivity * lightColour;
}