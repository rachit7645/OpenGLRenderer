#version 430 core

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(location = 0) in vec2 position;

out      vec2 txCoords;
out flat mat4 invProj;
out flat mat4 invView;

void main()
{
	// Set position
	gl_Position = vec4(position, 0.0f, 1.0f);
	// Set texture coordinates
	txCoords = position + vec2(1.0f);
	txCoords = txCoords / 2.0f;
	// Set inverse matrices
	invProj = inverse(projectionMatrix);
	invView = inverse(viewMatrix);
}