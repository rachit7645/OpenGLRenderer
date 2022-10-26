#version 430 core

// Constants
const float AMBIENT_STRENGTH = 0.2f;
const float MIN_SPECULAR     = 0.0f;
const int   MAX_LIGHTS       = 4;

const int   MAX_LAYER_COUNT = 16;
const float MIN_BIAS        = 0.005f;
const float MAX_BIAS        = 0.05f;
const float SHADOW_AMOUNT   = 0.35f;
const float BIAS_MODIFIER   = 0.5f;
const float PCF_COUNT       = 2.0f;
const float TOTAL_TEXELS    = (PCF_COUNT * 2.0f - 1.0f) * (PCF_COUNT * 2.0f - 1.0f);

// Structs

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

struct LightData
{
	float attFactor;
	vec3  lightDir;
	vec4  ambient;
	vec4  diffuse;
	vec4  specular;
};

struct GBuffer
{
	vec3  fragPos;
	vec3  normal;
	vec3  albedo;
	float specular;
	float shineDamper;
	float reflectivity;
};

// UBOs and SSBOs

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
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
	vec4 resolution;
};

layout (std140, binding = 4) uniform ShadowBuffer
{
	int   cascadeCount;
	mat4  shadowMatrices[MAX_LAYER_COUNT];
	float cascadeDistances[MAX_LAYER_COUNT];
};

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2DArray shadowMap;

// Fragment outputs
out vec4 outColor;

// GBuffer functions
GBuffer GetGBufferData();

// Branchless functions
vec4 WhenNotEqual(vec4 x, vec4 y);
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);

// Lighting functions
LightData CalculateLightData(int index, GBuffer gBuffer);

// Light component functions
float CalculateAttFactor(int index, GBuffer gBuffer);
vec4  CalculateAmbient(int index);
vec4  CalculateDiffuse(int index, LightData lightData, GBuffer gBuffer);
vec4  CalculateSpecular(int index, LightData lightData, GBuffer gBuffer);

// Shadow functions
int   GetCurrentLayer(GBuffer gBuffer);
float CalculateBias(int layer, LightData lightData, GBuffer gBuffer);
float CalculateShadow(LightData lightData, GBuffer gBuffer);

void main()
{
	// GBuffer data
	GBuffer gBuffer = GetGBufferData();
	// Light data
	LightData lightData[MAX_LIGHTS];

	// Total light factors
	vec4 ambient = vec4(0.0f);
	vec4 diffuse = vec4(0.0f);

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		// Calculate light data
		lightData[i] = CalculateLightData(i, gBuffer);
		// Add to total
		ambient += lightData[i].ambient;
		diffuse += lightData[i].diffuse;
	}

	// Output lighting
	outColor = ambient + diffuse * (1.0f - CalculateShadow(lightData[0], gBuffer));
}

GBuffer GetGBufferData()
{
	GBuffer gBuffer;
	// Retrieve data from G-buffer
	vec4 gPos  = texture(gPosition,   txCoords);
	vec4 gNorm = texture(gNormal,     txCoords);
	vec4 gAlb  = texture(gAlbedoSpec, txCoords);
	// Position + Shine Damper
	gBuffer.fragPos     = gPos.rgb;
	gBuffer.shineDamper = gPos.a;
	// Normal + Reflectivity
	gBuffer.normal       = gNorm.rgb;
	gBuffer.reflectivity = gNorm.a;
	// Albedo + Specular
	gBuffer.albedo   = gAlb.rgb;
	gBuffer.specular = gAlb.a;
	// Return
	return gBuffer;
}

LightData CalculateLightData(int index, GBuffer gBuffer)
{
	LightData data;
	// Light direction
	data.lightDir = normalize(lights[index].position.xyz - gBuffer.fragPos);
	// Light attenuation factor
	data.attFactor = CalculateAttFactor(index, gBuffer);
	// Light ambient factor
	data.ambient = CalculateAmbient(index)
				 * vec4(gBuffer.albedo, 1.0f)
				 * data.attFactor;
	// Light diffuse factor
	data.diffuse = CalculateDiffuse(index, data, gBuffer)
				 * vec4(gBuffer.albedo, 1.0f)
				 * data.attFactor;
	// Light specular factor
	data.specular = CalculateSpecular(index, data, gBuffer)
				  * data.attFactor
				  * vec4(gBuffer.specular)
				  * WhenNotEqual(data.diffuse, vec4(0.0f));
	// Return
	return data;
}

float CalculateAttFactor(int index, GBuffer gBuffer)
{
	vec4  ATT       = lights[index].attenuation;
	float distance  = length(lights[index].position.xyz - gBuffer.fragPos);
	float attFactor = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	return 1.0f / attFactor;
}

vec4 CalculateAmbient(int index)
{
	return vec4(AMBIENT_STRENGTH * lights[index].ambient.rgb, 1.0f);
}

vec4 CalculateDiffuse(int index, LightData lightData, GBuffer gBuffer)
{
	float nDot1      = dot(gBuffer.normal, lightData.lightDir);
	float brightness = max(nDot1, 0.0f);
	return vec4(brightness * lights[index].diffuse.rgb, 1.0f);
}

vec4 CalculateSpecular(int index, LightData lightData, GBuffer gBuffer)
{
	vec3 toCameraVector  = normalize(cameraPos.xyz - gBuffer.fragPos);
	vec3 halfwayDir      = normalize(lightData.lightDir + toCameraVector);
	float specularFactor = dot(halfwayDir, gBuffer.normal);
	specularFactor       = max(specularFactor, MIN_SPECULAR);
	float dampedFactor   = pow(specularFactor, gBuffer.shineDamper);
	return vec4(dampedFactor * gBuffer.reflectivity * lights[index].specular.rgb, 1.0f);
}

// FIXME: Remove branching
int GetCurrentLayer(GBuffer gBuffer)
{
	vec4  viewPosition = viewMatrix * vec4(gBuffer.fragPos, 1.0f);
	float depthValue   = abs(viewPosition.z);

	int layer = -1;
	for (int i = 0; i < cascadeCount; ++i)
	{
		if (depthValue < cascadeDistances[i])
		{
			layer = i;
			break;
		}
	}

	if (layer == -1)
	{
		layer = cascadeCount;
	}

	return layer;
}

// FIXME: Remove branching
float CalculateBias(int layer, LightData lightData, GBuffer gBuffer)
{
	float bias = max(MAX_BIAS * (1.0f - dot(gBuffer.normal, lightData.lightDir)), MIN_BIAS);

	if (layer == cascadeCount)
	{
		bias *= 1.0f / (resolution.w * BIAS_MODIFIER);
	}
	else
	{
		bias *= 1.0f / (cascadeDistances[layer] * BIAS_MODIFIER);
	}

	return bias;
}

float CalculateShadow(LightData lightData, GBuffer gBuffer)
{
	int layer = GetCurrentLayer(gBuffer);

	vec4 lightSpacePos = shadowMatrices[layer] * vec4(gBuffer.fragPos, 1.0f);
	vec3 projCoords    = lightSpacePos.xyz / lightSpacePos.w;
	projCoords         = projCoords * 0.5f + 0.5f;
	float currentDepth = projCoords.z;
	float bias         = CalculateBias(layer, lightData, gBuffer);

	float shadow    = 0.0f;
	vec2  texelSize = 1.0f / vec2(textureSize(shadowMap, 0));

	for (float x = -PCF_COUNT; x <= PCF_COUNT; ++x)
	{
		for (float y = -PCF_COUNT; y <= PCF_COUNT; ++y)
		{
			float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
			shadow        += WhenGreater(vec4(currentDepth - bias), vec4(pcfDepth)).x;
		}
	}

	shadow /= TOTAL_TEXELS;
	shadow *= SHADOW_AMOUNT;
	return shadow * WhenLesser(vec4(currentDepth), vec4(1.0f)).x;
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

// Branchless implementation of
// if (x > y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenGreater(vec4 x, vec4 y)
{
	return max(sign(x - y), 0.0f);
}

// Branchless implementation of
// if (x < y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenLesser(vec4 x, vec4 y)
{
	return max(sign(y - x), 0.0f);
}