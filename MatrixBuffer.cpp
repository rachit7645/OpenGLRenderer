#include "MatrixBuffer.h"

using namespace Renderer;

MatrixBuffer::MatrixBuffer() : UniformBuffer(0, sizeof(glm::mat4) * 2) {}

void MatrixBuffer::LoadView(const glm::mat4& view)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void MatrixBuffer::LoadProjection(const glm::mat4& projection)
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}