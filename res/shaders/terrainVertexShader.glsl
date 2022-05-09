#version 330 core

const float FOG_DENSITY	   = 0.0035f;
const float FOG_GRADIENT   = 1.5f;
const float TEXTURE_TILING = 40.0f;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;

out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

void CalculateLighting(vec4 worldPosition);
void CalculateVisibility(vec4 positionRelativeToCamera);

void main() 
{
	vec4 worldPosition = modelMatrix * vec4(position, 1.0f);
	vec4 positionRelativeToCamera = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCamera;
	pass_textureCoords = textureCoords * TEXTURE_TILING;
	
	CalculateLighting(worldPosition);
	CalculateVisibility(positionRelativeToCamera);
}

void CalculateLighting(vec4 worldPosition)
{
	surfaceNormal = (modelMatrix * vec4(normal, 0.0f)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(viewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz - worldPosition.xyz;
}

void CalculateVisibility(vec4 positionRelativeToCamera)
{
	float distance = length(positionRelativeToCamera.xyz);
	visibility = exp(-pow((distance * FOG_DENSITY), FOG_GRADIENT));
	visibility = clamp(visibility, 0.0f, 1.0f);
}