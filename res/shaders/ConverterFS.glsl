#version 430 core

const vec2 INVERSE_ATAN = vec2(0.1591f, 0.3183f);

in vec3 worldPos;

uniform sampler2D hdrMap;

out vec4 outColor;

vec2 SampleSphericalMap(vec3 v);

void main()
{
	// Sample hdr map
	vec2 uv       = SampleSphericalMap(normalize(worldPos));
	vec3 hdrColor = texture(hdrMap, uv).rgb;
	// Output
	outColor = vec4(hdrColor, 1.0f);
}

vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv     *= INVERSE_ATAN;
	uv     += 0.5f;
	return uv;
}