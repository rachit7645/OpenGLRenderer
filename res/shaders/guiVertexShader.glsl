#version 430 core

layout(location = 0) in vec2 position;

uniform mat4 modelMatrix;

out vec2 txCoords;

void main()
{
    gl_Position = modelMatrix * vec4(position, 0.0f, 1.0f);
    txCoords    = vec2((position.x + 1.0f) / 2.0f, 1 - (position.y + 1.0f) / 2.0f);
}