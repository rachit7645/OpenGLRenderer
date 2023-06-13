#version 430 core

// Shared buffer
layout(std140, binding = 2) uniform Shared
{
    vec4 clipPlane;
    vec4 cameraPos;
    vec4 resolution;
};

// Geometry inputs
in vec3 fragPos;

// Uniforms
uniform vec3 lightPos;

// Entry point
void main()
{
    // Calulate distance between fragment and light
    float lightDistance = length(fragPos.xyz - lightPos);

    // Map to [0, 1] range by dividing by far plane
    lightDistance = lightDistance / resolution.w;

    // Write modified depth
    gl_FragDepth = lightDistance;
}