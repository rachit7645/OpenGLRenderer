#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 worldPos;

void main()
{
	worldPos    = position;
	gl_Position = projection * view * vec4(position, 1.0f);
	gl_Position = gl_Position.xyww;
}