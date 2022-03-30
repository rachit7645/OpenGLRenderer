#version 330 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;

uniform sampler2D modelTexture;
uniform vec3 lightColour;

out vec4 out_color;

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, 0.2f);
	vec3 diffuse = brightness * lightColour;

	out_color = vec4(diffuse, 1.0f) * texture(modelTexture, pass_textureCoords);
}