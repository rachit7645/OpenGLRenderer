#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 textureCoords;

void main()
{
	// Set the z position to the w position
	// This makes it so that during perspective division z / w = w / w = 1.0f
	// This means that the driver will (hopefully) use early depth test
	// On my machine, this gives a 15-20 fps boost
	gl_Position = (projectionMatrix * viewMatrix * vec4(position, 1.0f)).xyww;
	textureCoords = position;
}