#version 430 core

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

// Vertex inputs
layout (location = 0) in vec3 position;

// Uniforms
uniform mat4 modelMatrix;

// Vertex outputs
out vec3 txCoords;

void main()
{
	// Disable translation
	mat4 rotView = mat4(mat3(viewMatrix));
	// Convert to clip space
	gl_Position = projectionMatrix * rotView * modelMatrix * vec4(position, 1.0f);
	// Early depth test
	gl_Position = gl_Position.xyww;
	// TxCoords
	txCoords = position;
}