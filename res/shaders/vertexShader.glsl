#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 pass_textureCoords;

void main() 
{
	gl_Position = vec4(position, 1.0f);
	pass_textureCoords = textureCoords;
}