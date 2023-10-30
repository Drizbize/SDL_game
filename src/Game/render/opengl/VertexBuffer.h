#pragma once

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void allocate(unsigned int size);
	void push(const void* data) const;

	void bind() const;
	void unbind() const;
private:
	unsigned int m_id;
	unsigned int m_size;
};