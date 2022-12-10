#version 430 core

struct Instance
{
	mat4 modelMatrix;
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

out vec2 txCoords;
out vec3 worldPos;
out vec3 unitNormal;
out mat3 TBNMatrix;

void main()
{
	// Transform vertex
	vec4 fragPos = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	worldPos     = fragPos.xyz;
	gl_Position  = projectionMatrix * viewMatrix * fragPos;
	// Pass through texture coords
	txCoords = texCoords;
	// Transform normal
	mat4 normalMatrix = instances[gl_InstanceID].modelMatrix;
	unitNormal        = normalize(normalMatrix * vec4(normal, 0.0f)).xyz;
	// Calculate TBN matrix
	vec3 N    = unitNormal;
	vec3 T    = normalize(normalMatrix * vec4(tangent, 0.0f)).xyz;
	T         = normalize(T - dot(T, N) * N);
	vec3 B    = cross(N, T);
	TBNMatrix = mat3(T, B, N);
}