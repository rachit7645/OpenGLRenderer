#version 430 core

// Constants
const float INV_GAMMA_FACTOR = 1.0f / 2.2f;

// Vertex inputs
in vec2 txCoords;

// Uniforms
uniform sampler2D lightingBuffer;

// Fragment outputs
out vec4 outColor;

// Entry point
void main()
{
	// Get lighting color
	vec3 color = texture(lightingBuffer, txCoords).rgb;
	// Perform Reinhard operator
	color = color / (color + vec3(1.0f));
	// Gamma correction
	color = pow(color, vec3(INV_GAMMA_FACTOR));
	// Set output
	outColor = vec4(color, 1.0f);
}