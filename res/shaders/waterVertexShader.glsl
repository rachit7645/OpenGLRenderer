#version 430 core

const float TEXTURE_TILING = 6.0f;

layout(location = 0) in vec2 position;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
};

uniform mat4 modelMatrix;

out vec2 txCoords;
out vec3 toCameraVector;
out vec4 clipSpace;

void main()
{
	vec4 worldPosition = modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
	clipSpace          = projectionMatrix * viewMatrix * worldPosition;
	gl_Position        = clipSpace;
	txCoords           = vec2(position.x / 2.0f + 0.5f, position.y / 2.0f + 0.5f);
	txCoords           = txCoords * TEXTURE_TILING;
	toCameraVector     = normalize(cameraPos.xyz - worldPosition.xyz);
}