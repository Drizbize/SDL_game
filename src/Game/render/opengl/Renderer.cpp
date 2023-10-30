#include "Renderer.h"

#include "glad/glad.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include <array>

Renderer::Renderer(const std::string& filepath)
	: m_shader(filepath)
{
}

void Renderer::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_offset = 0;
	m_indicesCount = 0;
	m_capacity = 2000 * 4;
	m_vertexes.reserve(sizeof(Vertex) * m_capacity);

	m_layout.push<float>(3);
	m_vb.allocate(m_capacity * sizeof(Vertex));
	m_va.addBuffer(m_vb, m_layout);
}

void Renderer::addObject(float x, float y)
{
	auto quad = createQuad(x, y);
	memcpy(m_vertexes.data() + m_offset, quad.data(), quad.size() * sizeof(Vertex));
	m_offset += quad.size();
	m_indicesCount += 6;
}

void Renderer::draw() const
{
	m_shader.bind();
	m_va.bind();

	m_vb.push(m_vertexes.data());

	std::vector<unsigned int> indices;
	indices.reserve(m_indicesCount);

	int step = 0;

	for (unsigned int i = 0; i < m_indicesCount; i += 6)
	{
		indices.push_back(step);
		indices.push_back(step + 1);
		indices.push_back(step + 2);
		indices.push_back(step + 2);
		indices.push_back(step + 3);
		indices.push_back(step);

		step += 4;
	}		   

	IndexBuffer ib(indices.data(), m_indicesCount);
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr);

	// clear everything
	m_offset = 0;
	m_indicesCount = 0;
	m_vertexes.clear();
}

std::array<Vertex, 4> Renderer::createQuad(float x, float y)
{
	float size = 0.02f;

	Vertex v0;
	v0.position = { x, y, 0.0f };
	//v0.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vertex v1;
	v1.position = { x + size, y, 0.0f };
	//v1.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vertex v2;
	v2.position = { x + size, y + size, 0.0f };
	//v2.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vertex v3;
	v3.position = { x, y + size, 0.0f };
	//v3.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	return { v0, v1, v2, v3 };
}
