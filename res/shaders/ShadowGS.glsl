#version 430 core

// TODO: Make invocation count dynamic

// Constants
const int MAX_LAYER_COUNT = 16;

// Geometry inputs & outputs
layout(triangles,      invocations  = 5) in;
layout(triangle_strip, max_vertices = 3) out;

// Shadow buffer
layout (std140, binding = 4) uniform ShadowBuffer
{
	int   cascadeCount;
	mat4  shadowMatrices[MAX_LAYER_COUNT];
	float cascadeDistances[MAX_LAYER_COUNT];
};

// Entry point
void main()
{
	// For each vertex
	for (int i = 0; i < 3; ++i)
	{
		// Calculate position
		gl_Position = shadowMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		// Calculate shadow map layer
		gl_Layer = gl_InvocationID;
		// Output vertex
		EmitVertex();
	}
	// Finish primitive
	EndPrimitive();
}