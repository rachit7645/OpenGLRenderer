#version 430 core

// TODO: Separate point and directional lights

// Constants
const float PI                 = 3.14159265359;
const int   MAX_LIGHTS         = 4;
const float MAX_REFLECTION_LOD = 4.0f;
const int   MAX_LAYER_COUNT    = 16;
const float MIN_BIAS           = 0.005f;
const float MAX_BIAS           = 0.05f;
const float SHADOW_AMOUNT      = 0.16f;
const float BIAS_MODIFIER      = 0.5f;
const float PCF_COUNT          = 1.5f;
const float TOTAL_TEXELS       = (PCF_COUNT * 2.0f - 1.0f) * (PCF_COUNT * 2.0f - 1.0f);

struct Light
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

// UBOs and SSBOs

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	int   numLights;
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

// GBuffer functions
GBuffer GetGBufferData();
vec3    ReconstructPosition();

// Branchless functions
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);

// Light functions
vec3  GetNormalFromMap(GBuffer gBuffer);
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

	// Normal
	vec3 N = GetNormalFromMap(gBuffer);
	// Camera vector
	vec3 V = normalize(cameraPos.xyz - gBuffer.fragPos);
	// Reflection vector
	vec3 R = reflect(-V, N);

	// Metallic coefficient
	vec3 F0 = vec3(0.04f);
	F0      = mix(F0, gBuffer.albedo.rgb, gBuffer.metallic);

	// Total light
	vec3 Lo = vec3(0.0f);

	for (int i = 0; i < numLights; ++i)
	{
		// Irradiance
		vec3  L           = normalize(lights[i].position.xyz - gBuffer.fragPos);
		vec3  H           = normalize(V + L);
		float distance    = length(lights[i].position.xyz - gBuffer.fragPos);
		vec3  ATT         = lights[i].attenuation.xyz;
		float attenuation = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
		attenuation       = 1.0f / attenuation;
		vec3  radiance    = lights[i].color.rgb * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, gBuffer.roughness);
		float G   = GeometrySmith(N, V, L, gBuffer.roughness);
		vec3  F   = FresnelSchlick(max(dot(H, V), 0.0f), F0, gBuffer.roughness);

		// Combine specular
		vec3 numerator = NDF * G * F;
		// To prevent division by zero, divide by at least 0.0001f (close enough)
		float denominator = max(4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f), 0.0001f);
		vec3  specular    = numerator / denominator;

		// Diffuse energy conservation
		vec3 kS = F;
		vec3 kD = vec3(1.0f) - kS;
		kD     *= 1.0f - gBuffer.metallic;

		// Add everthing up to Lo
		float NdotL = max(dot(N, L), 0.0f);
		Lo         += (kD * gBuffer.albedo / PI + specular) * radiance * NdotL;
	}

	// Ambient energy conservation
	vec3 F  = FresnelSchlick(max(dot(N, V), 0.0f), F0, gBuffer.roughness);
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;
	kD     *= 1.0f - gBuffer.metallic;

	// Irradiance
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse    = irradiance * gBuffer.albedo;

	// Specular
	vec3 prefilteredColor = textureLod(prefilterMap, R, gBuffer.roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf             = texture(brdfLUT, vec2(max(dot(N, V), 0.0f), gBuffer.roughness)).rg;
	vec3 specular         = prefilteredColor * (F * brdf.x + brdf.y);

	// Add up ambient
	vec3 ambient = (kD * diffuse + specular) * gBuffer.ao;

	// Add up color
	vec3 color   = ambient + Lo;
	vec3 L       = normalize(lights[0].position.xyz - gBuffer.fragPos);
	color       *= 1.0f - CalculateShadow(L, gBuffer);

	// HDR tonemapping
	color = color / (color + vec3(1.0f));
	// Gamma correction
	color = pow(color, vec3(1.0f / 2.2f));

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
float CalculateBias(int layer, vec3 lightDir, GBuffer gBuffer)
{
	float bias = max(MAX_BIAS * (1.0f - dot(gBuffer.normal, lightDir)), MIN_BIAS);

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