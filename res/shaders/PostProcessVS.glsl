#version 430 core

// Vertex attibutes
layout(location = 0) in vec2 position;

// Vertex outputs
out vec2 txCoords;

// Entry point
void main()
{
    // Set position
    gl_Position = vec4(position, 0.0f, 1.0f).xyww;
    // Generate texture coordinates
    txCoords = position + vec2(1.0f);
    txCoords = txCoords / 2.0f;
}