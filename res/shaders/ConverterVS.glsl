#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 worldPos;

void main()
{
	worldPos = position;
	// Remove translation from view
	mat4 rotView = mat4(mat3(view));
	// Convert to clip space
	vec4 clipPos = projection * rotView * vec4(position, 1.0f);
	// Early depth test
	gl_Position = clipPos.xyww;
}