#version 330 core

in vec3 position;
in vec2 textureCoords;

uniform mat4 modelMatrix;

out vec2 pass_textureCoords;

void main() 
{
	gl_Position = modelMatrix * vec4(position, 1.0f);
	pass_textureCoords = textureCoords;
}