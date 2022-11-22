#version 430 core

// TODO: Separate point and directional lights

// Constants
const float PI                 = 3.14159265359;
const int   MAX_LIGHTS         = 4;
const float MAX_REFLECTION_LOD = 4.0f;
const float INV_GAMMA_FACTOR   = 1.0f / 2.2f;
const int   MAX_LAYER_COUNT    = 16;
const float MIN_BIAS           = 0.005f;
const float MAX_BIAS           = 0.05f;
const float SHADOW_AMOUNT      = 0.16f;
const float BIAS_MODIFIER      = 0.5f;
const float PCF_COUNT          = 1.5f;
const float TOTAL_TEXELS       = (PCF_COUNT * 2.0f - 1.0f) * (PCF_COUNT * 2.0f - 1.0f);

struct DirLight
{
	vec4 position;
	vec4 color;
};

struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 attenuation;
};

struct GBuffer
{
	vec3  fragPos;
	vec3  normal;
	vec3  albedo;
	vec3  normalMap;
	float metallic;
	float roughness;
	float ao;
};

struct SharedData
{
	vec3 N;
	vec3 V;
	vec3 R;
	vec3 F0;
};

struct LightInfo
{
	vec3  position;
	vec3  color;
	float attenuation;
};

// UBOs and SSBOs

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	// Directional lights
	int      numDirLights;
	DirLight dirLights[MAX_LIGHTS];
	// Point lights
	int        numPointLights;
	PointLight pointLights[MAX_LIGHTS];
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
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
in      vec2 txCoords;
in flat mat4 invProj;
in flat mat4 invView;

// Samplers
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gNormalMap;
uniform sampler2D gDepth;
uniform sampler2D brdfLUT;
// Array samplers
uniform sampler2DArray shadowMap;
// CubeMap samplers
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;

// Fragment outputs
out vec4 outColor;

// Data functions
GBuffer    GetGBufferData();
SharedData GetSharedData(GBuffer gBuffer);
LightInfo  GetPointLightInfo(int index, GBuffer gBuffer);

// Branchless functions
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);
vec4 WhenEqual(vec4 x, vec4 y);

// Light functions
vec3 CalculateLight(SharedData sharedData, GBuffer gBuffer, LightInfo lightInfo);
vec3 CalculateAmbient(SharedData sharedData, GBuffer gBuffer);
vec3 GetNormalFromMap(GBuffer gBuffer);
vec3 ReconstructPosition();

// PBR Functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3  FresnelSchlick(float cosTheta, vec3 F0, float roughness);

// Shadow functions
int   GetCurrentLayer(GBuffer gBuffer);
float CalculateBias(int layer, vec3 lightDir, GBuffer gBuffer);
float CalculateShadow(vec3 lightDir, GBuffer gBuffer);

void main()
{
	// GBuffer data
	GBuffer gBuffer = GetGBufferData();
	// Shared Data
	SharedData sharedData = GetSharedData(gBuffer);

	// Total light
	vec3 Lo = vec3(0.0f);

	// Calculate point lights
	for (int i = 0; i < numPointLights; ++i)
	{
		// Calculate light data
		LightInfo info = GetPointLightInfo(i, gBuffer);
		// Calculate lighting
		Lo += CalculateLight(sharedData, gBuffer, info);
	}

	// Calculate ambient
	vec3 ambient = CalculateAmbient(sharedData, gBuffer);
	// Add up color
	vec3 color   = ambient + Lo;
	// Calculate shadow
	vec3 L       = normalize(pointLights[0].position.xyz - gBuffer.fragPos);
	color       *= 1.0f - CalculateShadow(L, gBuffer);

	// Perform Reinhard operator
	color = color / (color + vec3(1.0f));
	// Gamma correction
	color = pow(color, vec3(INV_GAMMA_FACTOR));

	// Output color
	outColor = vec4(color, 1.0f);
}

GBuffer GetGBufferData()
{
	GBuffer gBuffer;
	// Retrieve data from G-buffer
	vec4 gNorm    = texture(gNormal,    txCoords);
	vec4 gAlb     = texture(gAlbedo,    txCoords);
	vec4 gNormMap = texture(gNormalMap, txCoords);
	// Reconstruct Position
	gBuffer.fragPos = ReconstructPosition();
	// Normal + Roughness
	gBuffer.normal    = gNorm.rgb;
	gBuffer.roughness = gNorm.a;
	// Albedo + Metallic
	gBuffer.albedo   = gAlb.rgb;
	gBuffer.metallic = gAlb.a;
	// Normal Map + Ambient Occlusion
	gBuffer.normalMap = gNormMap.rgb;
	gBuffer.ao        = gNorm.a;
	// Return
	return gBuffer;
}

SharedData GetSharedData(GBuffer gBuffer)
{
	SharedData sharedData;
	// Normal
	sharedData.N = GetNormalFromMap(gBuffer);
	// Camera vector
	sharedData.V = normalize(cameraPos.xyz - gBuffer.fragPos);
	// Reflection vector
	sharedData.R = reflect(-sharedData.V, sharedData.N);
	// Metallic coefficient
	sharedData.F0 = mix(vec3(0.04f), gBuffer.albedo, gBuffer.metallic);
	// Return shared data
	return sharedData;
}

LightInfo GetPointLightInfo(int index, GBuffer gBuffer)
{
	LightInfo info;
	// Position
	info.position = pointLights[index].position.xyz;
	// Color
	info.color = pointLights[index].color.rgb;
	// Attenuation
	float distance   = length(info.position - gBuffer.fragPos);
	vec3 ATT         = pointLights[index].attenuation.xyz;
	info.attenuation = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	info.attenuation = 1.0f / info.attenuation;
	// Return
	return info;
}

vec3 ReconstructPosition()
{
	// Get depth
	float depth = texture(gDepth, txCoords).r;
	// Invert projection
	vec4 projectedPos = invProj * (vec4(txCoords, depth, 1.0f) * 2.0f - 1.0f);
	// Invert view
	vec3 viewPos = projectedPos.xyz / projectedPos.w;
	// Get world position
	vec3 worldPos = vec3(invView * vec4(viewPos, 1.0f));
	// Return
	return worldPos;
}

vec3 GetNormalFromMap(GBuffer gBuffer)
{
	// Tangent normal from normal map
	vec3 tangentNormal = gBuffer.normalMap * 2.0f - 1.0f;

	// Take the deriviatives
	vec3 Q1  = dFdx(gBuffer.fragPos);
	vec3 Q2  = dFdy(gBuffer.fragPos);
	vec2 st1 = dFdx(txCoords);
	vec2 st2 = dFdy(txCoords);

	// Calculate TBN matrix
	vec3 N   = gBuffer.normal;
	vec3 T   = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B   = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	// Return world space normal
	return normalize(TBN * tangentNormal);
}

vec3 CalculateLight(SharedData sharedData, GBuffer gBuffer, LightInfo lightInfo)
{
	// Irradiance
	vec3  L        = normalize(lightInfo.position - gBuffer.fragPos);
	vec3  H        = normalize(sharedData.V + L);
	float distance = length(lightInfo.position - gBuffer.fragPos);
	vec3  radiance = lightInfo.color * lightInfo.attenuation;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(sharedData.N, H, gBuffer.roughness);
	float G   = GeometrySmith(sharedData.N, sharedData.V, L, gBuffer.roughness);
	vec3  F   = FresnelSchlick(max(dot(H, sharedData.V), 0.0f), sharedData.F0, gBuffer.roughness);

	// Combine specular
	vec3 numerator = NDF * G * F;
	// To prevent division by zero, divide by at least 0.0001f (close enough)
	float denominator = max(4.0f * max(dot(sharedData.N, sharedData.V), 0.0f) * max(dot(sharedData.N, L), 0.0f), 0.0001f);
	vec3  specular    = numerator / denominator;

	// Diffuse energy conservation
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;
	kD     *= 1.0f - gBuffer.metallic;

	// Add everthing up to Lo
	float NdotL = max(dot(sharedData.N, L), 0.0f);
	return (kD * gBuffer.albedo / PI + specular) * radiance * NdotL;
}

vec3 CalculateAmbient(SharedData sharedData, GBuffer gBuffer)
{
	// Ambient energy conservation
	vec3 F  = FresnelSchlick(max(dot(sharedData.N, sharedData.V), 0.0f), sharedData.F0, gBuffer.roughness);
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;
	kD     *= 1.0f - gBuffer.metallic;

	// Irradiance
	vec3 irradiance = texture(irradianceMap, sharedData.N).rgb;
	vec3 diffuse    = irradiance * gBuffer.albedo;

	// Specular
	vec3 prefilteredColor = textureLod(prefilterMap, sharedData.R, gBuffer.roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf             = texture(brdfLUT, vec2(max(dot(sharedData.N, sharedData.V), 0.0f), gBuffer.roughness)).rg;
	vec3 specular         = prefilteredColor * (F * brdf.x + brdf.y);

	// Add up ambient
	vec3 ambient = (kD * diffuse + specular) * gBuffer.ao;

	// Return
	return ambient;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a      = roughness * roughness;
	float a2     = a * a;
	float NdotH  = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom       = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r) / 8.0f;

	float nom   = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	float ggx2  = GeometrySchlickGGX(NdotV, roughness);
	float ggx1  = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

int GetCurrentLayer(GBuffer gBuffer)
{
	vec4  viewPosition = viewMatrix * vec4(gBuffer.fragPos, 1.0f);
	float depthValue   = abs(viewPosition.z);

	// Set it to this as default to avoid a branch
	int layer = cascadeCount;

	for (int i = 0; i < cascadeCount; ++i)
	{
		if (depthValue < cascadeDistances[i])
		{
			layer = i;
			break;
		}
	}

	return layer;
}

float CalculateBias(int layer, vec3 lightDir, GBuffer gBuffer)
{
	// Using normal map's normal
	float bias = max(MAX_BIAS * (1.0f - dot(gBuffer.normalMap, lightDir)), MIN_BIAS);
	// Select bias type
	float selection = WhenEqual(vec4(layer), vec4(cascadeCount)).x;
	selection       = clamp(selection, 0.0f, 1.0f);

	// Calculate both cases
	float bias1 = bias * 1.0f / (resolution.w * BIAS_MODIFIER);
	float bias2 = bias * 1.0f / (cascadeDistances[layer] * BIAS_MODIFIER);

	// CASE 1: selection == 0.0f
	// bias1 will cancel out, leaving bias2
	// CASE 2: selection == 1.0f
	// bias2 will cancel out, leaving bias 1
	bias = bias1 * selection + bias2 * (1.0f - selection);

	return bias;
}

float CalculateShadow(vec3 lightDir, GBuffer gBuffer)
{
	int layer = GetCurrentLayer(gBuffer);

	vec4 lightSpacePos = shadowMatrices[layer] * vec4(gBuffer.fragPos, 1.0f);
	vec3 projCoords    = lightSpacePos.xyz / lightSpacePos.w;
	projCoords         = projCoords * 0.5f + 0.5f;
	float currentDepth = projCoords.z;
	float bias         = CalculateBias(layer, lightDir, gBuffer);

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

// Branchless implementation of
// if (x == y)
//	 return 1;
// else
// 	 return 0;
vec4 WhenEqual(vec4 x, vec4 y)
{
	return 1.0f - abs(sign(x - y));
}