#version 420 core

const float MIN_BRIGHTNESS = 0.2f;
const float MIN_SPECULAR   = 0.0f;
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

in float visibility;
in vec2 pass_textureCoords;
in vec3 unitNormal;
in vec3 unitCameraVector;
in vec3 unitLightVector[MAX_LIGHTS];

uniform sampler2D modelTexture;
uniform vec4 skyColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 outColor;

vec3 CalculateDiffuse(int index);
vec3 CalculateSpecular(int index);

void main()
{
	vec4 textureColour = texture(modelTexture, pass_textureCoords);
	// HACK: Alpha transparency check, should probably be in a separate shader
	if (textureColour.a < 0.5f)
		discard;

	vec3 totalDiffuse = vec3(0.0f); vec3 totalSpecular = vec3(0.0f);
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		totalDiffuse += CalculateDiffuse(i);
		totalSpecular += CalculateSpecular(i);
	}

	totalDiffuse = max(totalDiffuse, vec3(MIN_BRIGHTNESS));
	outColor = vec4(totalDiffuse, 1.0f) * textureColour + vec4(totalSpecular, 1.0f);
	outColor = mix(skyColour, outColor, visibility);
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
	specularFactor = max(specularFactor, 0.0f);
	float dampedFactor = pow(specularFactor, shineDamper);
	return dampedFactor * reflectivity * lights[index].colour.xyz;
}