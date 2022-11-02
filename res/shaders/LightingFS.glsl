#version 430 core

// Constants
const int MAX_LIGHTS = 4;

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
	vec4  albedo;
	vec3  normalMap;
	float metallic;
	float roughness;
	float ao;
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

// Samplers
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gNormalMap;
// Array samplers
uniform sampler2DArray shadowMap;

// Fragment outputs
out vec4 outColor;

// GBuffer functions
GBuffer GetGBufferData();

// Branchless functions
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);

// Shadow functions
int   GetCurrentLayer(GBuffer gBuffer);
float CalculateBias(int layer, LightData lightData, GBuffer gBuffer);
float CalculateShadow(LightData lightData, GBuffer gBuffer);

void main()
{
	// GBuffer data
	GBuffer gBuffer = GetGBufferData();

	outColor = gBuffer.albedo;
}

GBuffer GetGBufferData()
{
	GBuffer gBuffer;
	// Retrieve data from G-buffer
	vec4 gPos     = texture(gPosition,  txCoords);
	vec4 gNorm    = texture(gNormal,    txCoords);
	vec4 gAlb     = texture(gAlbedo,    txCoords);
	vec4 gNormMap = texture(gNormalMap, txCoords);
	// Position + Metallic
	gBuffer.fragPos  = gPos.rgb;
	gBuffer.metallic = gPos.a;
	// Normal + Roughness
	gBuffer.normal    = gNorm.rgb;
	gBuffer.roughness = gNorm.a;
	// Albedo
	gBuffer.albedo = gAlb;
	// Normal Map + Ambient Occlusion
	gBuffer.normalMap = gNormMap.rgb;
	gBuffer.ao        = gNorm.a;
	// Return
	return gBuffer;
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