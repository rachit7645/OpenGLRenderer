#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 textureCoords;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0f);
	textureCoords = position;
}