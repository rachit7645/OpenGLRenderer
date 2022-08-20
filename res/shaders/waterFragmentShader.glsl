#version 430 core

in vec2 txCoords;
in vec4 clipSpace;

uniform sampler2D reflectionTx;
uniform sampler2D refractionTx;

out vec4 outColor;

void main()
{
	vec2 NDC             = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 reflectTxCoords = vec2(NDC.x, -NDC.y);
	vec2 refractTxCoords = vec2(NDC.x, NDC.y);

	vec4 reflectColor = texture(reflectionTx, reflectTxCoords);
	vec4 refractColor = texture(refractionTx, refractTxCoords);

	outColor = mix(reflectColor, refractColor, 0.5f);
}