#version 430 core

// Matrix buffer
layout(std140, binding = 0) uniform Matrices
{
	// Regular matrices
	mat4 projection;
	mat4 cameraView;
	// Inverse matrices
	mat4 invProjection;
	mat4 invCameraView;
};

// Vertex inputs
layout (location = 0) in vec3 position;

// Vertex outputs
out vec3 txCoords;

void main()
{
	// Disable translation
	mat4 rotView = mat4(mat3(cameraView));
	// Convert to clip space
	gl_Position = projection * rotView * vec4(position, 1.0f);
	// Early depth test
	gl_Position = gl_Position.xyww;
	// TxCoords
	txCoords = position;
}