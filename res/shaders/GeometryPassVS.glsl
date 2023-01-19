#version 430 core

// Entity instance
struct Instance
{
	mat4 modelMatrix;
};

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

// Instance data SSBO
layout(std430, binding = 3) readonly buffer InstanceData
{
	Instance instances[];
};

// Vertex inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

// Vertex outputs
out vec2 txCoords;
out vec3 worldPos;
out mat3 TBNMatrix;

// Entry point
void main()
{
	// Transform vertex by model matrix
	vec4 fragPos = instances[gl_InstanceID].modelMatrix * vec4(position, 1.0f);
	// Set world position
	worldPos = fragPos.xyz;
	// Transform from world to clip space
	gl_Position = projectionMatrix * viewMatrix * fragPos;
	// Pass through texture coords
	txCoords = texCoords;
	// Get normal matrix
	mat4 normalMatrix = instances[gl_InstanceID].modelMatrix;
	// Transform normal
	vec3 N = normalize(normalMatrix * vec4(normal, 0.0f)).xyz;
	// Transform tangent
	vec3 T = normalize(normalMatrix * vec4(tangent, 0.0f)).xyz;
	// Re-orthagonalize tangent
	T = normalize(T - dot(T, N) * N);
	// Calculate bitangent
	vec3 B = cross(N, T);
	// Compute TBN matrix
	TBNMatrix = mat3(T, B, N);
}