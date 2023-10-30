#include "VertexBuffer.h"

#include "glad/glad.h"

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::allocate(unsigned int size)
{
	bind();
	m_size = size;
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void VertexBuffer::push(const void* data) const
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_size, data);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
