#version 430 core

layout(location = 0) in vec2 position;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPos;
};

uniform mat4 modelMatrix;

out vec2 txCoords;
out vec4 clipSpace;

void main()
{
	clipSpace   = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
	gl_Position = clipSpace;
	txCoords    = vec2(position.x / 2.0f + 0.5f, position.y / 2.0f + 0.5f);
}