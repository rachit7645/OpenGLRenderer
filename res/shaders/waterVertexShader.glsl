#version 430 core

const float TEXTURE_TILING = 4.0f;
const int   MAX_LIGHTS     = 4;

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 attenuation;
};

layout(location = 0) in vec2 position;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140, binding = 1) uniform Lights
{
	Light lights[MAX_LIGHTS];
};

layout(std140, binding = 2) uniform Shared
{
	vec4 clipPlane;
	vec4 skyColor;
	vec4 cameraPos;
};

uniform mat4 modelMatrix;

out vec4 clipSpace;
out vec3 unitCameraVector;
out vec3 unitLightVector;
out vec2 txCoords;

void main()
{
	vec4 worldPosition = modelMatrix * vec4(position.x, 0.0f, position.y, 1.0f);
	clipSpace          = projectionMatrix * viewMatrix * worldPosition;
	gl_Position        = clipSpace;
	txCoords           = vec2(position.x / 2.0f + 0.5f, position.y / 2.0f + 0.5f);
	txCoords           = txCoords * TEXTURE_TILING;
	unitCameraVector   = normalize(cameraPos.xyz - worldPosition.xyz);
	unitLightVector    = normalize(lights[0].position.xyz - worldPosition.xyz);
}