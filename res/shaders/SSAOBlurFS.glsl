#version 430 core

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D ssaoInput;

// Fragment outputs
layout(location = 0) out float outColor;

// Entry point
void main()
{
	// Get texel size
	vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
	// Store result
	float result = 0.0f;

	// For each x in [-2, 2]
	for (int x = -2; x < 2; ++x)
	{
		// For each y in [-2, 2]
		for (int y = -2; y < 2; ++y)
		{
			// Calculate offset
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			// Add to result
			result += texture(ssaoInput, txCoords + offset).r;
		}
	}

	// Finalise result and output
	outColor = result / (4.0f * 4.0f);
}