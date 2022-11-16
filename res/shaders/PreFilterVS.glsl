#version 430 core

const float PI = 3.14159265359;

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform vec2 resolution;

out      vec3  worldPos;
out flat float saTexel;

void main()
{
	worldPos    = position;
	gl_Position = projection * view * vec4(position, 1.0f);
	gl_Position = gl_Position.xyww;
	// Fix for bright spots
	saTexel = 4.0f * PI / (6.0f * resolution.x * resolution.y);
}