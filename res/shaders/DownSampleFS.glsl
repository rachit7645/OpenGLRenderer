#version 430 core

// Constants
const float INV_GAMMA_FACTOR = 1.0f / 2.2f;

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D srcTexture;
uniform int       isFirstSample;

// Fragment outputs
layout (location = 0) out vec3 downsample;

// Karis Average functions
vec3  ToSRGB(vec3 color);
float ToLuminance(vec3 color);
float KarisAverage(vec3 color);

// Entry point
void main()
{
    // Get texel size
    vec2 srcTexelSize = 1.0f / vec2(textureSize(srcTexture, 0));

    // Get inputs
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    // Top row samples (A - B - C)
    vec3 a = texture(srcTexture, vec2(txCoords.x - 2.0f * x, txCoords.y + 2.0f * y)).rgb;
    vec3 b = texture(srcTexture, vec2(txCoords.x,            txCoords.y + 2.0f * y)).rgb;
    vec3 c = texture(srcTexture, vec2(txCoords.x + 2.0f * x, txCoords.y + 2.0f * y)).rgb;

    // Middle row samples (D - E - F)
    vec3 d = texture(srcTexture, vec2(txCoords.x - 2.0f * x, txCoords.y)).rgb;
    vec3 e = texture(srcTexture, vec2(txCoords.x,            txCoords.y)).rgb;
    vec3 f = texture(srcTexture, vec2(txCoords.x + 2.0f * x, txCoords.y)).rgb;

    // Bottom row samples (G - H - I)
    vec3 g = texture(srcTexture, vec2(txCoords.x - 2.0f * x, txCoords.y - 2.0f * y)).rgb;
    vec3 h = texture(srcTexture, vec2(txCoords.x,            txCoords.y - 2.0f * y)).rgb;
    vec3 i = texture(srcTexture, vec2(txCoords.x + 2.0f * x, txCoords.y - 2.0f * y)).rgb;

    // Second top row samples (- J - K -)
    vec3 j = texture(srcTexture, vec2(txCoords.x - x, txCoords.y + y)).rgb;
    vec3 k = texture(srcTexture, vec2(txCoords.x + x, txCoords.y + y)).rgb;

    // Second bottom row samples (- L - M -)
    vec3 l = texture(srcTexture, vec2(txCoords.x - x, txCoords.y - y)).rgb;
    vec3 m = texture(srcTexture, vec2(txCoords.x + x, txCoords.y - y)).rgb;

    if (isFirstSample == 1)
    {
        // Allocate groups
        vec3 groups[5];
        // Set groups
        groups[0] = (a + b + d + e) * (0.125f / 4.0f);
        groups[1] = (b + c + e + f) * (0.125f / 4.0f);
        groups[2] = (d + e + g + h) * (0.125f / 4.0f);
        groups[3] = (e + f + h + i) * (0.125f / 4.0f);
        groups[4] = (j + k + l + m) * (0.5f   / 4.0f);
        // Do Karis Average
        groups[0] *= KarisAverage(groups[0]);
        groups[1] *= KarisAverage(groups[1]);
        groups[2] *= KarisAverage(groups[2]);
        groups[3] *= KarisAverage(groups[3]);
        groups[4] *= KarisAverage(groups[4]);
        // Set output
        downsample = groups[0] + groups[1] + groups[2] + groups[3] + groups[4];
    }
    else
    {
        // Apply weighted distribution
        downsample  = e * 0.125f;
        downsample += (a + c + g + i) * 0.03125f;
        downsample += (b + d + f + h) * 0.0625f;
        downsample += (j + k + l + m) * 0.125f;
    }

    // Fix for black spots
    downsample = max(downsample, 0.0001f);
}

vec3 ToSRGB(vec3 color)
{
    // Do gamma correction
    return pow(color, vec3(INV_GAMMA_FACTOR));
}

float ToLuminance(vec3 color)
{
    // Convert to luma
    return dot(color, vec3(0.2126f, 0.7152f, 0.0722f));
}

float KarisAverage(vec3 color)
{
    // Get luma
    float luma = ToLuminance(ToSRGB(color)) * 0.25f;
    // Do karis average
    return 1.0f / (1.0f + luma);
}