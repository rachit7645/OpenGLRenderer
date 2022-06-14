#include "MatrixBuffer.h"

using namespace Renderer;

MatrixBuffer::MatrixBuffer()
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, id);
}

MatrixBuffer::~MatrixBuffer()
{
	glDeleteBuffers(1, &id);
}

void MatrixBuffer::Update()
{
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}