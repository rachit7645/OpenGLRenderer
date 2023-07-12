#version 430 core

// Approximation of inverse arc tangent
const vec2 INVERSE_ATAN = vec2(0.1591f, 0.3183f);

// Vertex inputs
in vec3 worldPos;

// Uniforms
uniform sampler2D hdrMap;

// Fragment outputs
layout (location = 0) out vec3 outColor;

// Conversion functions
vec2 GetSphericalMapUV(vec3 v);

// Entry point
void main()
{
    // Get normal
    vec3 normal = normalize(worldPos);
    // Get UV coords
    vec2 uv = GetSphericalMapUV(normal);
    // Sample at new UV coords
    vec4 hdrMap = texture(hdrMap, uv);
    // Set output
    outColor = hdrMap.rgb;
}

vec2 GetSphericalMapUV(vec3 v)
{
    // Convert to spherical coords
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv     *= INVERSE_ATAN;
    uv     += 0.5f;
    // Return
    return uv;
}