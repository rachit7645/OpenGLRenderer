#version 430 core

in vec2 txCoords;

uniform sampler2D guiTexture;

out vec4 outColor;

void main()
{
    outColor = texture(guiTexture, txCoords);
}