#version 430 core

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D srcTexture;
uniform float     filterRadius;

// Fragment outputs
layout (location = 0) out vec3 upsample;

// Entry point
void main()
{
	// Radius in txCoord space so that it varies per mip level
	float x = filterRadius;
	float y = filterRadius;

	// Top row samples (A - B - C)
	vec3 a = texture(srcTexture, vec2(txCoords.x - x, txCoords.y + y)).rgb;
	vec3 b = texture(srcTexture, vec2(txCoords.x,     txCoords.y + y)).rgb;
	vec3 c = texture(srcTexture, vec2(txCoords.x + x, txCoords.y + y)).rgb;

	// Middle row samples (D - E - F)
	vec3 d = texture(srcTexture, vec2(txCoords.x - x, txCoords.y)).rgb;
	vec3 e = texture(srcTexture, vec2(txCoords.x,     txCoords.y)).rgb;
	vec3 f = texture(srcTexture, vec2(txCoords.x + x, txCoords.y)).rgb;

	// Bottom row samples (G - H - I)
	vec3 g = texture(srcTexture, vec2(txCoords.x - x, txCoords.y - y)).rgb;
	vec3 h = texture(srcTexture, vec2(txCoords.x,     txCoords.y - y)).rgb;
	vec3 i = texture(srcTexture, vec2(txCoords.x + x, txCoords.y - y)).rgb;

	// Apply weighted distribution using a 3x3 tent filter
	upsample  = e * 4.0f;
	upsample += (b + d + f + h) * 2.0f;
	upsample += (a + c + g + i);
	upsample *= 1.0f / 16.0f;
}