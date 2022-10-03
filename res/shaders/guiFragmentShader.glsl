#version 430 core

in vec2 txCoords;

uniform sampler2D guiTexture;

out vec4 outColor;

void main()
{
    outColor = vec4(vec3(texture(guiTexture, txCoords).r), 1.0f);
}