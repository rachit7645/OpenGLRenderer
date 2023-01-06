#version 430 core

// Lighting constants
const float PI                 = 3.14159265359;
const int   MAX_LIGHTS         = 4;
const float MAX_REFLECTION_LOD = 4.0f;
// Shadow constants
const int   MAX_LAYER_COUNT = 16;
const float MIN_BIAS        = 0.005f;
const float MAX_BIAS        = 0.05f;
const float SHADOW_AMOUNT   = 0.16f;
const float BIAS_MODIFIER   = 0.35f;
const float PCF_COUNT       = 1.2f;
const float TOTAL_TEXELS    = (PCF_COUNT * 2.0f - 1.0f) * (PCF_COUNT * 2.0f - 1.0f);

// Directional Light
struct DirLight
{
	vec4 position;
	vec4 color;
	vec4 intensity;
};

// Point Light
struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 intensity;
	vec4 attenuation;
};

// Spot Light
struct SpotLight
{
	vec4 position;
	vec4 color;
	vec4 intensity;
	vec4 attenuation;
	vec4 direction;
	vec4 cutOff;
};

// Per-fragment GBuffer Data
struct GBuffer
{
	vec3  fragPos;
	vec3  normal;
	vec3  albedo;
	float metallic;
	float roughness;
	float ao;
};

// Per-fragment Shared Data
struct SharedData
{
	vec3 N;
	vec3 V;
	vec3 R;
	vec3 F0;
};

// Common Light Information
struct LightInfo
{
	vec3  position;
	vec3  color;
	vec3  distance;
	vec3  intensity;
	float attenuation;
	float spotIntensity;
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
	// Spot Lights
	int       numSpotLights;
	SpotLight spotLights[MAX_LIGHTS];
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
uniform sampler2D gMaterial;
uniform sampler2D gDepth;
uniform sampler2D brdfLUT;
// Array samplers
uniform sampler2DArray shadowMap;
// CubeMap samplers
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;

// Fragment outputs
out vec3 outColor;

// Data functions
GBuffer    GetGBufferData();
SharedData GetSharedData(GBuffer gBuffer);
LightInfo  GetDirLightInfo(int index);
LightInfo  GetPointLightInfo(int index, GBuffer gBuffer);
LightInfo  GetSpotLightInfo(int index, GBuffer gBuffer);

// Branchless functions
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);
vec4 WhenEqual(vec4 x, vec4 y);

// Light functions
vec3 CalculateLight(SharedData sharedData, GBuffer gBuffer, LightInfo lightInfo);
vec3 CalculateAmbient(SharedData sharedData, GBuffer gBuffer);
vec3 ReconstructPosition();
vec3 UnpackNormal(vec2 normal);

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

	// Calculate directional lights
	for (int i = 0; i < numDirLights; ++i)
	{
		// Calculate light data
		LightInfo info = GetDirLightInfo(i);
		// Calculate lighting
		Lo += CalculateLight(sharedData, gBuffer, info);
	}

	// Calculate point lights
	for (int i = 0; i < numPointLights; ++i)
	{
		// Calculate light data
		LightInfo info = GetPointLightInfo(i, gBuffer);
		// Calculate lighting
		Lo += CalculateLight(sharedData, gBuffer, info);
	}

	// Calculate spot lights
	for (int i = 0; i < numSpotLights; ++i)
	{
		// Calculate light data
		LightInfo info = GetSpotLightInfo(i, gBuffer);
		// Calculate lighting
		Lo += CalculateLight(sharedData, gBuffer, info);
	}

	// Calculate ambient
	vec3 ambient = CalculateAmbient(sharedData, gBuffer);
	// Add up color
	vec3 color = ambient + Lo;
	// Calculate shadow
	vec3 L  = normalize(-dirLights[0].position.xyz);
	color  *= 1.0f - CalculateShadow(L, gBuffer);

	// Output color
	outColor = color;
}

GBuffer GetGBufferData()
{
	GBuffer gBuffer;
	// Retrieve data from G-buffer
	vec4 gNorm = texture(gNormal,   txCoords);
	vec4 gAlb  = texture(gAlbedo,   txCoords);
	vec4 gMat  = texture(gMaterial, txCoords);
	// Reconstruct Position
	gBuffer.fragPos = ReconstructPosition();
	// Normal
	gBuffer.normal = UnpackNormal(gNorm.rg);
	// Albedo
	gBuffer.albedo = gAlb.rgb;
	// AO + Roughness + Metallic
	gBuffer.ao        = gMat.r;
	gBuffer.roughness = gMat.g;
	gBuffer.metallic  = gMat.b;
	// Return
	return gBuffer;
}

SharedData GetSharedData(GBuffer gBuffer)
{
	SharedData sharedData;
	// Normal
	sharedData.N = gBuffer.normal;
	// Camera vector
	sharedData.V = normalize(cameraPos.xyz - gBuffer.fragPos);
	// Reflection vector
	sharedData.R = reflect(-sharedData.V, sharedData.N);
	// Metallic coefficient
	sharedData.F0 = mix(vec3(0.04f), gBuffer.albedo, gBuffer.metallic);
	// Return shared data
	return sharedData;
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

vec3 UnpackNormal(vec2 normal)
{
	// Convert back to [-1, 1]
	vec2  fEnc = normal * 4.0f - 2.0f;
	float f    = dot(fEnc, fEnc);
	float g    = sqrt(1.0f - f / 4.0f);
	// Store result
	vec3 unpacked;
	unpacked.xy = fEnc * g;
	unpacked.z  = 1.0f - f / 2.0f;
	// Return
	return unpacked;
}

LightInfo GetDirLightInfo(int index)
{
	LightInfo info;
	// Position
	info.position = dirLights[index].position.xyz;
	// Color
	info.color = dirLights[index].color.rgb;
	// Distance
	info.distance = -info.position;
	// Intensity
	info.intensity = dirLights[index].intensity.xyz;
	// Attenuation
	info.attenuation = 1.0f;
	// Spot Intensity
	info.spotIntensity = 1.0f;
	// Return
	return info;
}

LightInfo GetPointLightInfo(int index, GBuffer gBuffer)
{
	LightInfo info;
	// Position
	info.position = pointLights[index].position.xyz;
	// Color
	info.color = pointLights[index].color.rgb;
	// Distance
	info.distance = info.position - gBuffer.fragPos;
	// Intensity
	info.intensity = pointLights[index].intensity.xyz;
	// Attenuation
	vec3  ATT        = pointLights[index].attenuation.xyz;
	float distance   = length(info.distance);
	info.attenuation = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	info.attenuation = 1.0f / info.attenuation;
	// Spot Intensity
	info.spotIntensity = 1.0f;
	// Return
	return info;
}

LightInfo GetSpotLightInfo(int index, GBuffer gBuffer)
{
	LightInfo info;
	// Position
	info.position = spotLights[index].position.xyz;
	// Color
	info.color = spotLights[index].color.rgb;
	// Distance
	info.distance = info.position - gBuffer.fragPos;
	// Intensity
	info.intensity = spotLights[index].intensity.xyz;
	// Attenuation
	vec3  ATT        = spotLights[index].attenuation.xyz;
	float distance   = length(info.distance);
	info.attenuation = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
	info.attenuation = 1.0f / info.attenuation;
	// Spot Intensity
	float theta        = dot(normalize(info.distance), normalize(-spotLights[index].direction.xyz));
	float epsilon      = spotLights[index].cutOff.x - spotLights[index].cutOff.y;
	info.spotIntensity = smoothstep(0.0f, 1.0f, (theta - spotLights[index].cutOff.y) / epsilon);
	// Return
	return info;
}

vec3 CalculateLight(SharedData sharedData, GBuffer gBuffer, LightInfo lightInfo)
{
	// Irradiance
	vec3 L        = normalize(lightInfo.distance);
	vec3 H        = normalize(sharedData.V + L);
	vec3 radiance = lightInfo.color * lightInfo.intensity * lightInfo.attenuation * lightInfo.spotIntensity;

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
	// Get view-space position
	vec4 viewPosition = viewMatrix * vec4(gBuffer.fragPos, 1.0f);
	// Get depth
	float depthValue = abs(viewPosition.z);
	// Set layer to the highest value
	int layer = cascadeCount;

	// For each cascade
	for (int i = 0; i < cascadeCount; ++i)
	{
		// If depth is less than distance
		if (depthValue < cascadeDistances[i])
		{
			// Select layer
			layer = i;
			// Exit loop
			break;
		}
	}

	return layer;
}

float CalculateBias(int layer, vec3 lightDir, GBuffer gBuffer)
{
	// Calculate slope-scaled bias
	float bias = max(MAX_BIAS * (1.0f - dot(gBuffer.normal, lightDir)), MIN_BIAS);
	// Select bias type
	float selection = WhenEqual(vec4(layer), vec4(cascadeCount)).x;
	selection       = clamp(selection, 0.0f, 1.0f);

	// Calculate both cases
	float bias1 = bias * 1.0f / (resolution.w * BIAS_MODIFIER);
	float bias2 = bias * 1.0f / (cascadeDistances[layer] * BIAS_MODIFIER);

	// CASE 1: selection == 0.0f
	// bias1 will cancel out, leaving bias 2
	// CASE 2: selection == 1.0f
	// bias2 will cancel out, leaving bias 1
	bias = bias1 * selection + bias2 * (1.0f - selection);

	return bias;
}

float CalculateShadow(vec3 lightDir, GBuffer gBuffer)
{
	// Calculate current cascade
	int layer = GetCurrentLayer(gBuffer);

	// Get shadow space position
	vec4 lightSpacePos = shadowMatrices[layer] * vec4(gBuffer.fragPos, 1.0f);
	// Perform perspective division
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords      = projCoords * 0.5f + 0.5f;
	// Calculate depth
	float currentDepth = projCoords.z;
	// Calculate bias
	float bias = CalculateBias(layer, lightDir, gBuffer);
	// Calculate texel size
	vec2 texelSize = 1.0f / vec2(textureSize(shadowMap, 0));

	// Store shadow
	float shadow = 0.0f;

	// For each x offset
	for (float x = -PCF_COUNT; x <= PCF_COUNT; ++x)
	{
		// For each y offset
		for (float y = -PCF_COUNT; y <= PCF_COUNT; ++y)
		{
			// Get offseted depth
			float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
			// Perform shadow comparision
			shadow += WhenGreater(vec4(currentDepth - bias), vec4(pcfDepth)).x;
		}
	}

	// Divide shadow by total texels
	shadow /= TOTAL_TEXELS;
	// Modify by shadow amount
	shadow *= SHADOW_AMOUNT;
	// Return if depth < 1.0f
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