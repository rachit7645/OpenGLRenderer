#version 420 core

layout(location = 0) in vec2 position;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPos;
};

uniform mat4 modelMatrix;

out vec2 txCoords;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
	txCoords    = vec2(position.x / 2.0f + 0.5f, position.y / 2.0f + 0.5f);
}