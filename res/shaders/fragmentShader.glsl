#version 430 core

// TODO: Gamma Correction?

const float AMBIENT_STRENGTH = 0.2f;
const float MIN_SPECULAR     = 0.0f;
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

in vec4  worldPosition;
in vec3  unitLightVector[MAX_LIGHTS];
in vec3  unitNormal;
in vec3  unitCameraVector;
in vec2  txCoords;
in float visibility;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec4  skyColour;
uniform float shineDamper;
uniform float reflectivity;

out vec4 outColor;

vec4 WhenNotEqual(vec4 x, vec4 y);

float CalculateAttFactor(int index);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index);
vec4  CalculateSpecular(int index);

void main()
{
	vec4 diffuseColor = texture(diffuseTexture, txCoords);
	// HACK: Alpha transparency check, should probably be in a separate shader
	if (diffuseColor.a < 0.5f)
	{
		discard;
	}
	vec4 specularColor = texture(specularTexture, txCoords);

	vec4 totalAmbient  = vec4(0.0f);	
	vec4 totalDiffuse  = vec4(0.0f);
	vec4 totalSpecular = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		float attFactor = CalculateAttFactor(i);
		totalAmbient  += CalculateAmbient(i)  * diffuseColor  * attFactor;
		vec4 curDiff  = CalculateDiffuse(i)   * diffuseColor  * attFactor;
		totalDiffuse  += curDiff;
		vec4 curSpec  = CalculateSpecular(i) * specularColor * attFactor;
		totalSpecular += curSpec * WhenNotEqual(curDiff, vec4(0.0));
	}

	// Add all lighting
	outColor = totalDiffuse + totalSpecular + totalAmbient;
	// Mix with fog colour
	outColor = mix(skyColour, outColor, visibility);
}

float CalculateAttFactor(int index)
{
	#define ATT lights[index].attenuation
	float distance  = length(lights[index].position.xyz - worldPosition.xyz);
	float attFactor = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
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

vec4 CalculateSpecular(int index)
{
	vec3 lightDirection  = unitLightVector[index];
	vec3 halfwayDir      = normalize(lightDirection + unitCameraVector);
	float specularFactor = dot(unitNormal, unitCameraVector);
	specularFactor       = max(specularFactor, MIN_SPECULAR);
	float dampedFactor   = pow(specularFactor, shineDamper);
	return vec4(dampedFactor * reflectivity * lights[index].specular.rgb, 1.0f);
}

// Branchless implementation of
// if (x != y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenNotEqual(vec4 x, vec4 y)
{
	return abs(sign(x - y));
}