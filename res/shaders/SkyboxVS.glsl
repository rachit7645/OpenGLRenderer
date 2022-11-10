#version 430 core

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout (location = 0) in vec3 position;

uniform mat4 modelMatrix;

out vec3 txCoords;

void main()
{
	// Disable translation
	mat4 rotView = mat4(mat3(viewMatrix));
	// Early depth test
	gl_Position = (projectionMatrix * rotView * modelMatrix * vec4(position, 1.0f)).xyww;
	txCoords    = position;
}