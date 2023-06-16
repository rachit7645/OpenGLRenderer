#version 430 core

// Constants
const int MAX_LIGHTS = 4;

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

// Omni Shadow buffer
layout (std140, binding = 5) uniform OmniShadowBuffer
{
    mat4 omniShadowMatrices[6];
    vec4 shadowPlanes;
    int  lightIndex;
};

// Geometry inputs
in vec3 fragPos;

// Entry point
void main()
{
    // Calulate distance between fragment and light
    float lightDistance = length(fragPos.xyz - pointLights[lightIndex].position.xyz);

    // Map to [0, 1] range by dividing by far plane
    lightDistance = lightDistance / shadowPlanes.y;

    // Write modified depth
    gl_FragDepth = lightDistance;
}