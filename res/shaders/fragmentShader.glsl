#version 330 core

in vec2 pass_textureCoords;
out vec4 out_color;

uniform sampler2D rtexture;

void main()
{
	out_color = texture(rtexture, pass_textureCoords);
}