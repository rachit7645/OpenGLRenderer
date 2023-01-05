#version 430 core

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D srcTexture;

// Fragment outputs
layout (location = 0) out vec3 downsample;

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
	vec3 d = texture(srcTexture, vec2(txCoords.x - 2*x, txCoords.y)).rgb;
	vec3 e = texture(srcTexture, vec2(txCoords.x,       txCoords.y)).rgb;
	vec3 f = texture(srcTexture, vec2(txCoords.x + 2*x, txCoords.y)).rgb;

	// Bottom row samples (G - H - I)
	vec3 g = texture(srcTexture, vec2(txCoords.x - 2*x, txCoords.y - 2*y)).rgb;
	vec3 h = texture(srcTexture, vec2(txCoords.x,       txCoords.y - 2*y)).rgb;
	vec3 i = texture(srcTexture, vec2(txCoords.x + 2*x, txCoords.y - 2*y)).rgb;

	// Second top row samples (- J - K -)
	vec3 j = texture(srcTexture, vec2(txCoords.x - x, txCoords.y + y)).rgb;
	vec3 k = texture(srcTexture, vec2(txCoords.x + x, txCoords.y + y)).rgb;

	// Second bottom row samples (- L - M -)
	vec3 l = texture(srcTexture, vec2(txCoords.x - x, txCoords.y - y)).rgb;
	vec3 m = texture(srcTexture, vec2(txCoords.x + x, txCoords.y - y)).rgb;

	// Apply weighted distribution
	downsample =  e * 0.125f;
	downsample += (a + c + g + i) * 0.03125f;
	downsample += (b + d + f + h) * 0.0625f;
	downsample += (j + k + l + m) * 0.125f;
}