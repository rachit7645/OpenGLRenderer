#version 430 core

// Constants
const float PI           = 3.14159265359;
const uint  SAMPLE_COUNT = 2048u;

// Vertex inputs
in vec3 worldPos;

// Uniforms
uniform samplerCube envMap;
uniform vec2        resolution;
uniform float       roughness;

// Vertex outputs
layout (location = 0) out vec3 outColor;

// IBL functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float RadicalInverse_VdC(uint bits);
vec2  Hammersley(uint i, uint N);
vec3  ImportanceSampleGGX(vec2 Xi, vec3 N, vec3 tangent, vec3 bitangent, float roughness);

// Entry point
void main()
{
    // Calculate normal
    vec3 N = normalize(worldPos);

    // Assume that the reflection, viewing and normal vector are the same
    vec3 R = N;
    vec3 V = R;

    // Tangent to World Space Sample Vector
    vec3 up        = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    // Store outputs
    vec3  prefilteredColor = vec3(0.0f);
    float totalWeight      = 0.0f;

    // For each sample
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // Importance Sampling
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, tangent, bitangent, roughness);
        vec3 L  = normalize(2.0f * dot(V, H) * H - V);

        // Dot between normal and lighting direction
        float NdotL = max(dot(N, L), 0.0f);

        // Required to fix heavy artifacts
        if(NdotL > 0.0f)
        {
            // Sample based on roughness/pdf
            float D     = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0f);
            float HdotV = max(dot(H, V), 0.0f);
            float pdf   = D * NdotH / (4.0f * HdotV) + 0.0001f;

            // Fix for extremely bright spots
            float saSample = 1.0f / (float(SAMPLE_COUNT) * pdf + 0.0001f);
            float saTexel  = 4.0f * PI / (6.0f * resolution.x * resolution.y);
            float mipLevel = roughness == 0.0f ? 0.0f : 0.5f * log2(saSample / saTexel);

            // Set outputs
            prefilteredColor += textureLod(envMap, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }

    // Finalise output color
    prefilteredColor = prefilteredColor / totalWeight;
    // Set output color
    outColor = prefilteredColor;
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

float RadicalInverse_VdC(uint bits)
{
    // No idea how this even works
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
    // Perform Hammersly function
    return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, vec3 tangent, vec3 bitangent, float roughness)
{
    // Square roughness as proposed by Disney
    float a = roughness * roughness;

    // Calculate trignometric angles
    float phi      = 2.0f * PI * Xi.x;
    float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    // Spherical to Cartesian Halfway Vector
    vec3 H = vec3
    (
        cos(phi) * sinTheta,
        sin(phi) * sinTheta,
        cosTheta
    );

    // Calculate sampler vectpr
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    // Normalise and return
    return normalize(sampleVec);
}