#version 430 core

// Constants
const float INV_GAMMA_FACTOR = 1.0f / 2.2f;

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D lightingBuffer;
uniform sampler2D bloomBuffer;
uniform float     bloomStrength;

// Fragment outputs
layout (location = 0) out vec4 outColor;

// Tone mappers
vec3 ToneMap(vec3 color);
// Gamma Correction
vec3 GammaCorrect(vec3 color);

// Entry point
void main()
{
    // Get lighting color
    vec3 hdrColor = texture(lightingBuffer, txCoords).rgb;
    // Get bloom color
    vec3 bloomColor = texture(bloomBuffer, txCoords).rgb;
    // Add bloom color
    vec3 color = mix(hdrColor, bloomColor, bloomStrength);
    // Convert to LDR & SRGB
    color = GammaCorrect(ToneMap(color));
    // Set output
    outColor = vec4(color, 1.0f);
}

vec3 ToneMap(vec3 color)
{
    // ACES tone map operator
    return clamp((color * (2.51f * color + 0.03f)) / (color * (2.43f * color + 0.59f) + 0.14f), 0.0f, 1.0f);
}

vec3 GammaCorrect(vec3 color)
{
    // Convert to SRGB
    return pow(color, vec3(INV_GAMMA_FACTOR));
}