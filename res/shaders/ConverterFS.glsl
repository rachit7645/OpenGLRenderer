#version 430 core

in vec3 worldPos;

uniform samplerCube hdrMap;

out vec4 outColor;

void main()
{
	// Sample hdr map
	vec3 hdrColor = texture(hdrMap, worldPos).rgb;
	// Convert to LDR
	hdrColor = hdrColor / (hdrColor + vec3(1.0f));
	hdrColor = pow(hdrColor, vec3(1.0f / 2.2f));
	// Output
	outColor = vec4(hdrColor, 1.0f);
}