#version 450 core

// Vertex inputs
layout (location = 0) in vec3 position;

// Uniforms
uniform mat4 projection;
uniform mat4 view;

// Vertex outputs
out vec3 worldPos;

// Entry point
void main()
{
	// Set position
	worldPos    = position;
	gl_Position = projection * view * vec4(position, 1.0f);
	// Trick to guarantee early depth test
	gl_Position = gl_Position.xyww;
}