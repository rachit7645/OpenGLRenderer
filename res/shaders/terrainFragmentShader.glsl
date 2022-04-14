#version 330 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform sampler2D modelTexture;

uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 out_color;

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, 0.15f);
	vec3 diffuse = brightness * lightColour;

	vec3 unitCameraVector = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = dot(reflectedLightDirection, unitCameraVector);
	specularFactor = max(specularFactor, 0.0f);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightColour;

	out_color = vec4(diffuse, 1.0f) * texture(modelTexture, pass_textureCoords) + vec4(finalSpecular, 1.0f);
}