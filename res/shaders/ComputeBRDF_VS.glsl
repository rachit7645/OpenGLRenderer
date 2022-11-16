#version 430 core

const vec3 N = vec3(0.0f, 0.0f, 1.0f);

layout(location = 0) in vec2 position;

out      vec2 txCoords;
out flat vec3 tangent;
out flat vec3 bitangent;

void main()
{
	gl_Position = vec4(position, 0.0f, 1.0f).xyww;
	txCoords    = position + vec2(1.0f);
	txCoords    = txCoords / 2.0f;
	// Tangent to World Space Conversion
	vec3 up   = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	tangent   = normalize(cross(up, N));
	bitangent = cross(N, tangent);
}