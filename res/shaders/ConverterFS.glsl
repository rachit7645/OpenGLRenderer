#version 430 core

const vec2 INVERSE_ATAN = vec2(0.1591f, 0.3183f);

in vec3 worldPos;

uniform sampler2D hdrMap;

out vec3 outColor;

vec2 GetSphericalMapUV(vec3 v);

void main()
{
	outColor = texture(hdrMap, GetSphericalMapUV(normalize(worldPos))).rgb;
}

vec2 GetSphericalMapUV(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv     *= INVERSE_ATAN;
	uv     += 0.5f;
	return uv;
}