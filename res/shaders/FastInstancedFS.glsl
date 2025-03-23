#version 430 core

// Constants
const float MAX_REFLECTION_LOD = 4.0f;

// Vertex inputs
in vec2 txCoords;
in vec3 unitNormal;
in vec3 viewDir;
in vec4 worldPosition;

// Uniforms
uniform sampler2D   albedoMap;
uniform sampler2D   aoRghMtlMap;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

// Fragment outputs
layout(location = 0) out vec3 outColor;

// Lighting functions
vec3 CalculateAmbient(vec3 albedo, vec3 aoRghMtl);
vec3 FresnelSchlick(float cosTheta, vec3 F0, float roughness);

// Entry point
void main()
{
    // Fetch albedo color
    vec3 albedo = texture(albedoMap, txCoords).rgb;
    // Fetch material data
    vec3 aoRghMtl = texture(aoRghMtlMap, txCoords).rgb;
    // Set output to ambient light color
    outColor = CalculateAmbient(albedo, aoRghMtl);
}

vec3 CalculateAmbient(vec3 albedo, vec3 aoRghMtl)
{
    // AO
    float ao = aoRghMtl.r;
    // Roughness
    float roughness = aoRghMtl.g;
    // Metallic
    float metallic = aoRghMtl.b;

    // Normal
    vec3 N = unitNormal;
    // Camera vector
    vec3 V = viewDir;
    // Reflection vector
    vec3 R = reflect(-V, N);

    // Metallic coefficient
    vec3 F0 = mix(vec3(0.04f), albedo, metallic);
    // Dot products
    float NDotV = max(dot(N, V), 0.0f);

    // Ambient energy conservation
    vec3 F  = FresnelSchlick(NDotV, F0, roughness);
    vec3 kS = F;
    vec3 kD = vec3(1.0f) - kS;
    kD     *= 1.0f - metallic;

    // Irradiance
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    // Specular
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf             = texture(brdfLUT, vec2(NDotV, roughness)).rg;
    vec3 specular         = prefilteredColor * (F * brdf.x + brdf.y);

    // Add up ambient
    vec3 ambient = (kD * diffuse + specular) * ao;

    // Return
    return ambient;
}

// Fresnel equation function with injected roughness paramenter for IBL
vec3 FresnelSchlick(float cosTheta, vec3 F0, float roughness)
{
    // Clamp to avoid artifacts
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}