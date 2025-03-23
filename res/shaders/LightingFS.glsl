#version 430 core

// Lighting constants
const float PI                 = 3.14159265359;
const int   MAX_LIGHTS         = 4;
const float MAX_REFLECTION_LOD = 4.0f;

// Shadow constants
const int   MAX_LAYER_COUNT = 16;
const float MIN_BIAS        = 0.005f;
const float MAX_BIAS        = 0.05f;
const float BIAS_MODIFIER   = 0.35f;

// Point shadow constants
const int   NUM_SAMPLES = 20;
const float POINT_BIAS  = 0.15f;

// Spot shadow constants
const float MIN_SPOT_BIAS = 0.000005f;
const float MAX_SPOT_BIAS = 0.00025f;

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
    vec3  emmisive;
    float metallic;
    float roughness;
    float ao;
};

// Per-fragment Shared Data
struct SharedData
{
    vec3 N;
    vec3 V;
    vec3 F0;
};

// Common Light Information
struct LightInfo
{
    vec3 L;
    vec3 radiance;
};

// Point Light Shadow Struct
struct PointShadow
{
    mat4 omniShadowMatrices[6];
    vec4 shadowPlanes;
    int  lightIndex;
};

// Spot light shadow struct
struct SpotShadow
{
    mat4 shadowMatrix;
    int  lightIndex;
};

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
    // Regular matrices
    mat4 projection;
    mat4 cameraView;
    // Inverse matrices
    mat4 invProjection;
    mat4 invCameraView;
};

// Lights buffer
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

// Shared buffer
layout(std140, binding = 2) uniform Shared
{
    vec4 clipPlane;
    vec4 cameraPos;
    vec4 resolution;
};

// Shadow buffer
layout (std140, binding = 4) uniform ShadowBuffer
{
    int   cascadeCount;
    mat4  shadowMatrices[MAX_LAYER_COUNT];
    float cascadeDistances[MAX_LAYER_COUNT];
};

// Omni Shadow buffer
layout (std140, binding = 5) uniform OmniShadowBuffer
{
    PointShadow omniShadowMaps[MAX_LIGHTS];
    int         currentPointIndex;
};

// Spot shadow buffer
layout(std140, binding = 6) uniform SpotShadowBuffer
{
    SpotShadow spotShadowMaps[MAX_LIGHTS];
    int        currentSpotIndex;
};

// Vertex inputs
in vec2 txCoords;

// GBuffer samplers
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gEmmisive;
uniform sampler2D gDepth;
// IBL samplers
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;
// Other samplers
uniform sampler2DArrayShadow shadowMap;
uniform samplerCubeArray     pointShadowMap;
uniform sampler2DArrayShadow spotShadowMap;

// Fragment outputs
layout(location = 0) out vec3 outColor;

// Data functions
GBuffer    GetGBufferData();
SharedData GetSharedData(GBuffer gBuffer);
LightInfo  GetDirLightInfo(int index);
LightInfo  GetPointLightInfo(int index, GBuffer gBuffer);
LightInfo  GetSpotLightInfo(int index, GBuffer gBuffer);

// Special light functions
float CalculateAttenuation(vec3 position, vec3 ATT, GBuffer gBuffer);
float CalculateSpotIntensity(vec3 L, vec3 direction, vec2 cutOff, GBuffer gBuffer);

// Memory compression functions
vec3 ReconstructPosition();
vec3 UnpackNormal(vec2 pNormal);

// Light functions
vec3 CalculateLight(SharedData sharedData, GBuffer gBuffer, LightInfo lightInfo);
vec3 CalculateAmbient(SharedData sharedData, GBuffer gBuffer);

// PBR Functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3  FresnelSchlick(float cosTheta, vec3 F0);
vec3  FresnelSchlick(float cosTheta, vec3 F0, float roughness);

// Shadow functions
float TanArcCos(float x);
int   GetCurrentLayer(GBuffer gBuffer);
float CalculateBias(int layer, vec3 lightDir, GBuffer gBuffer);
float CalculateShadow(vec3 lightDir, GBuffer gBuffer);

// Point shadow functions
float CalculatePointShadow(PointShadow shadowMap, GBuffer gBuffer);

// Spot shadow functions
float CalculateSpotShadow(SpotShadow shadowMap, GBuffer gBuffer);

// Branchless functions
vec4 WhenGreater(vec4 x, vec4 y);
vec4 WhenLesser(vec4 x, vec4 y);
vec4 WhenEqual(vec4 x, vec4 y);

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
        // If light is sun
        float shadow = CalculateShadow(info.L, gBuffer) * WhenEqual(vec4(i), vec4(0)).x;
        // Calculate lighting
        Lo += CalculateLight(sharedData, gBuffer, info) * (1.0f - shadow);
    }

    // Calculate point lights
    for (int i = 0; i < numPointLights; ++i)
    {
        // Calculate light data
        LightInfo info = GetPointLightInfo(i, gBuffer);
        // Calculate shadows
        float shadow = CalculatePointShadow(omniShadowMaps[i], gBuffer);
        // Calculate lighting
        Lo += CalculateLight(sharedData, gBuffer, info) * (1.0f - shadow);
    }

    // Calculate spot lights
    for (int i = 0; i < numSpotLights; ++i)
    {
        // Calculate light data
        LightInfo info = GetSpotLightInfo(i, gBuffer);
        // Calculate shadow
        float shadow = CalculateSpotShadow(spotShadowMaps[i], gBuffer);
        // Calculate lighting
        Lo += CalculateLight(sharedData, gBuffer, info) * (1.0f - shadow);
    }

    // Calculate ambient
    vec3 ambient = CalculateAmbient(sharedData, gBuffer);
    // Add up color
    vec3 color = ambient + Lo;
    // Apply emmision
    color = color + gBuffer.emmisive;

    // Output color
    outColor = color;
}

GBuffer GetGBufferData()
{
    GBuffer gBuffer;
    // Retrieve data from textures
    vec4 gNorm = texture(gNormal,   txCoords);
    vec4 gAlb  = texture(gAlbedo,   txCoords);
    vec4 gEmm  = texture(gEmmisive, txCoords);
    // Reconstruct Position
    gBuffer.fragPos = ReconstructPosition();
    // Normal + AO + Roughness
    gBuffer.normal    = UnpackNormal(gNorm.rg);
    gBuffer.ao        = gNorm.b;
    gBuffer.roughness = gNorm.a;
    // Albedo + Metallic
    gBuffer.albedo   = gAlb.rgb;
    gBuffer.metallic = gAlb.a;
    // Emmisive
    gBuffer.emmisive = gEmm.rgb;
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
    // Metallic coefficient
    sharedData.F0 = mix(vec3(0.04f), gBuffer.albedo, gBuffer.metallic);
    // Return shared data
    return sharedData;
}

LightInfo GetDirLightInfo(int index)
{
    LightInfo info;
    // Get light
    DirLight light = dirLights[index];
    // Light vector
    info.L = normalize(-light.position.xyz);
    // Radiance
    info.radiance = light.color.rgb * light.intensity.xyz;
    // Return
    return info;
}

LightInfo GetPointLightInfo(int index, GBuffer gBuffer)
{
    LightInfo info;
    // Get light
    PointLight light = pointLights[index];
    // Light vector
    info.L = normalize(light.position.xyz - gBuffer.fragPos);
    // Attenuation
    float attenuation = CalculateAttenuation(light.position.xyz, light.attenuation.xyz, gBuffer);
    // Radiance
    info.radiance = light.color.rgb * light.intensity.xyz * attenuation;
    // Return
    return info;
}

LightInfo GetSpotLightInfo(int index, GBuffer gBuffer)
{
    LightInfo info;
    // Get light
    SpotLight light = spotLights[index];
    // Light vector
    info.L = normalize(light.position.xyz - gBuffer.fragPos);
    // Attenuation
    float attenuation = CalculateAttenuation(light.position.xyz, light.attenuation.xyz, gBuffer);
    // Spot intensity
    float intensity = CalculateSpotIntensity(info.L, light.direction.xyz, light.cutOff.xy, gBuffer);
    // Radiance
    info.radiance = light.color.rgb * light.intensity.xyz * attenuation * intensity;
    // Return
    return info;
}

float CalculateAttenuation(vec3 position, vec3 ATT, GBuffer gBuffer)
{
    // Get distance
    float distance = length(position - gBuffer.fragPos);
    // Calculate attenuation
    float attenuation = ATT.x + (ATT.y * distance) + (ATT.z * distance * distance);
    // Make attenuation multiplicable
    attenuation = 1.0f / max(attenuation, 0.0001f);
    // Return
    return attenuation;
}

float CalculateSpotIntensity(vec3 L, vec3 direction, vec2 cutOff, GBuffer gBuffer)
{
    // Calculate angle
    float theta = dot(L, normalize(-direction));
    // Calculate cut off epsilon
    float epsilon = cutOff.x - cutOff.y;
    // Calculate intensity
    float intensity = smoothstep(0.0f, 1.0f, (theta - cutOff.y) / epsilon);
    // Return
    return intensity;
}

vec3 ReconstructPosition()
{
    // Get depth
    float depth = texture(gDepth, txCoords).r;
    // Invert projection
    vec4 projectedPos = invProjection * (vec4(txCoords, depth, 1.0f) * 2.0f - 1.0f);
    // Invert view
    vec3 viewPos = projectedPos.xyz / projectedPos.w;
    // Get world position
    vec3 worldPos = vec3(invCameraView * vec4(viewPos, 1.0f));
    // Return
    return worldPos;
}

// FIXME: Remove conditionals
vec3 UnpackNormal(vec2 pNormal)
{
    // Convert range of packed normal
    pNormal = pNormal * 2.0f - 1.0f;
    // Create new normal
    vec3 normal = vec3(pNormal.x, pNormal.y, 1.0f - abs(pNormal.x) - abs(pNormal.y));
    // Calculate flags
    float flag = max(-normal.z, 0.0f);
    // X check
    normal.x += normal.x >= 0.0f ? -flag : flag;
    // Y check
    normal.y += normal.y >= 0.0f ? -flag : flag;
    // Return normalised normal
    return normalize(normal);
}

vec3 CalculateLight(SharedData sharedData, GBuffer gBuffer, LightInfo lightInfo)
{
    // Calculate half-way vector
    vec3 H = normalize(sharedData.V + lightInfo.L);
    // Calculate dots
    float NdotL = max(dot(sharedData.N, lightInfo.L), 0.0f);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(sharedData.N, H, gBuffer.roughness);
    float G   = GeometrySmith(sharedData.N, sharedData.V, lightInfo.L, gBuffer.roughness);
    vec3  F   = FresnelSchlick(max(dot(H, sharedData.V), 0.0f), sharedData.F0);

    // Combine specular
    vec3  numerator   = NDF * G * F;
    float denominator = 4.0f * max(dot(sharedData.N, sharedData.V), 0.0f) * max(dot(sharedData.N, lightInfo.L), 0.0f);
    // To prevent division by zero, divide by epsilon
    denominator   = max(denominator, 0.0001f);
    vec3 specular = numerator / denominator;

    // Diffuse energy conservation
    vec3 kS = F;
    vec3 kD = vec3(1.0f) - kS;
    kD     *= 1.0f - gBuffer.metallic;

    // Add everthing up to Lo
    return (kD * gBuffer.albedo / PI + specular) * lightInfo.radiance * NdotL;
}

vec3 CalculateAmbient(SharedData sharedData, GBuffer gBuffer)
{
    // Reflection vector
    vec3 R = reflect(-sharedData.V, sharedData.N);

    // Ambient energy conservation
    vec3 F  = FresnelSchlick(max(dot(sharedData.N, sharedData.V), 0.0f), sharedData.F0, gBuffer.roughness);
    vec3 kS = F;
    vec3 kD = vec3(1.0f) - kS;
    kD     *= 1.0f - gBuffer.metallic;

    // Irradiance
    vec3 irradiance = texture(irradianceMap, sharedData.N).rgb;
    vec3 diffuse    = irradiance * gBuffer.albedo;

    // Specular
    vec3 prefilteredColor = textureLod(prefilterMap, R, gBuffer.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf             = texture(brdfLUT, vec2(max(dot(sharedData.N, sharedData.V), 0.0f), gBuffer.roughness)).rg;
    vec3 specular         = prefilteredColor * (F * brdf.x + brdf.y);

    // Add up ambient
    vec3 ambient = (kD * diffuse + specular) * gBuffer.ao;

    // Return
    return ambient;
}

// Trowbridge-Reitz Normal Distribution Function
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    // Square roughness as proposed by Disney
    float a = roughness * roughness;

    // Calculate squares
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    // Calculate numerator and denominator
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom       = PI * denom * denom;

    // Divide and return
    return nom / denom;
}

// Schlick Geometry Function
float GeometrySchlickGGX(float NdotV, float roughness)
{
    // Calculate various parameters
    float r = (roughness + 1.0f);
    // Square roughness as proposed by Disney
    float k = (r * r) / 8.0f;

    // Calculate numerator and denominator
    float nom   = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    // Divide and return
    return nom / denom;
}

// Smith's method to combine Geometry functions
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    // Take dot products
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);

    // Calculate both geometry schlick outputs
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    // Combine and return
    return ggx1 * ggx2;
}

// Fresnel equation function
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    // Clamp to avoid artifacts
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

// Fresnel equation function with injected roughness paramenter for IBL
vec3 FresnelSchlick(float cosTheta, vec3 F0, float roughness)
{
    // Clamp to avoid artifacts
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

int GetCurrentLayer(GBuffer gBuffer)
{
    // Get view-space position
    vec4 viewPosition = cameraView * vec4(gBuffer.fragPos, 1.0f);
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

    // Return
    return layer;
}

// Faster version of the tangent of inverse cosine function
float TanArcCos(float x)
{
    // tan(acos(x)) = sqrt(1 - x^2) / x
    return sqrt(1.0f - (x * x)) / x;
}

float CalculateBias(int layer, vec3 lightDir, GBuffer gBuffer)
{
    // Calculate slope-scaled bias
    float cosTheta = clamp(dot(gBuffer.normal, lightDir), 0.0f, 1.0f);
    float bias     = MIN_BIAS * TanArcCos(cosTheta);
    bias           = clamp(bias, 0.0f, MAX_BIAS);

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

    // Return
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

    // Calculate shadow
    float shadow = texture(shadowMap, vec4(projCoords.xy, layer, currentDepth - bias));
    // Return if depth < 1.0f
    return shadow * WhenLesser(vec4(currentDepth), vec4(1.0f)).x;
}

float CalculatePointShadow(PointShadow shadowMap, GBuffer gBuffer)
{
    // Sample vectors for soft shadows
    const vec3 GRID_SAMPLING_DISK[NUM_SAMPLES] = vec3[]
    (
        vec3(1.0f, 1.0f,  1.0f), vec3( 1.0f, -1.0f,  1.0f), vec3(-1.0f, -1.0f,  1.0f), vec3(-1.0f, 1.0f,  1.0f),
        vec3(1.0f, 1.0f, -1.0f), vec3( 1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f), vec3(-1.0f, 1.0f, -1.0f),
        vec3(1.0f, 1.0f,  0.0f), vec3( 1.0f, -1.0f,  0.0f), vec3(-1.0f, -1.0f,  0.0f), vec3(-1.0f, 1.0f,  0.0f),
        vec3(1.0f, 0.0f,  1.0f), vec3(-1.0f,  0.0f,  1.0f), vec3( 1.0f,  0.0f, -1.0f), vec3(-1.0f, 0.0f, -1.0f),
        vec3(0.0f, 1.0f,  1.0f), vec3( 0.0f, -1.0f,  1.0f), vec3( 0.0f, -1.0f, -1.0f), vec3( 0.0f, 1.0f, -1.0f)
    );

    // Get light index
    int lightIndex = shadowMap.lightIndex;
    // Get light position
    vec3 lightPos = pointLights[lightIndex].position.xyz;
    // Get shadow planes
    vec2 planes = omniShadowMaps[lightIndex].shadowPlanes.xy;

    // Get vector from frament to light
    vec3 fragToLight = gBuffer.fragPos - lightPos;
    // Get current linear depth
    float currentDepth = length(fragToLight);

    // Store shadow
    float shadow = 0.0f;
    // Calculate distance from camera
    float viewDistance = length(cameraPos.xyz - gBuffer.fragPos);
    // Calculate samplinbg disk radius
    float diskRadius = (1.0f + (viewDistance / planes.y)) / 25.0f;

    // For each sample
    for(int i = 0; i < NUM_SAMPLES; ++i)
    {
        // Get closest depth
        float closestDepth = texture(pointShadowMap, vec4(fragToLight + GRID_SAMPLING_DISK[i] * diskRadius, lightIndex)).r;
        // Undo previous mapping
        closestDepth *= planes.y;
        // Check for shadow
        shadow += 1.0f * WhenGreater(vec4(currentDepth - POINT_BIAS), vec4(closestDepth)).x;
    }

    // Divide shadow amounht by number of samples
    shadow /= float(NUM_SAMPLES);
    // Return shadow
    return shadow;
}

float CalculateSpotShadow(SpotShadow shadowMap, GBuffer gBuffer)
{
    // Get light index
    int lightIndex = shadowMap.lightIndex;
    // Get light position
    vec3 lightPos = spotLights[lightIndex].position.xyz;

    // Get shadow space position
    vec4 lightSpacePos = shadowMap.shadowMatrix * vec4(gBuffer.fragPos, 1.0f);
    // Perform perspective division
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords      = projCoords * 0.5f + 0.5f;

    // Calculate depth
    float currentDepth = projCoords.z;

    // Calculate slope-scaled bias
    float cosTheta = clamp(dot(gBuffer.normal, normalize(lightPos)), 0.0f, 1.0f);
    float bias     = MIN_SPOT_BIAS * TanArcCos(cosTheta);
    bias           = clamp(bias, 0.0f, MAX_SPOT_BIAS);

    // Calculate shadow
    float shadow = texture(spotShadowMap, vec4(projCoords.xy, lightIndex, currentDepth - bias));
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