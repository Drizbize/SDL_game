#include "Renderer.h"

#include "glad/glad.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include <array>

Renderer::Renderer(const std::string& filepath)
	: m_shader(filepath)
{
}

void Renderer::init(float width, float height)
{
	m_width = width;
	m_height = height;
	m_zoom = 1.f;
	m_view = { 0.0f, 0.0f, 0.0f };

	resize(width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_offset = 0;
	m_indicesCount = 0;
	const int quads = 30000;
	m_capacity = quads * 4;
	m_vertexes.reserve(sizeof(Vertex) * m_capacity);

	m_layout.push<float>(3);
	m_vb.allocate(m_capacity * sizeof(Vertex));
	m_va.addBuffer(m_vb, m_layout);

	//m_proj = glm::ortho(0.f, m_width, 0.f, m_height, -1.0f, 1.0f);
	//m_shader.setUniformMat4("u_MVP", m_proj);

	m_indices.reserve(quads * 6);
	int step = 0;

	for (unsigned int i = 0; i < quads * 6; i += 6)
	{
		m_indices.push_back(step);
		m_indices.push_back(step + 1);
		m_indices.push_back(step + 2);
		m_indices.push_back(step + 2);
		m_indices.push_back(step + 3);
		m_indices.push_back(step);

		step += 4;
	}
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

	IndexBuffer ib(m_indices.data(), m_indicesCount);
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr);

	// clear everything
	m_offset = 0;
	m_indicesCount = 0;
	m_vertexes.clear();
}

void Renderer::moveCamera(int x, int y)
{
	m_view.x += x;
	m_view.y -= y;

	glm::mat4 view = glm::translate(glm::mat4(1.f), m_view);
	m_shader.setUniformMat4("u_MVP", m_proj * view);
}

void Renderer::resize(int width, int height)
{
	glViewport(0, 0, width, height);
	m_width = width;
	m_height = height;

	m_proj = glm::ortho(0.f, m_width * m_zoom, 0.f, m_height * m_zoom, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.f), m_view);
	m_shader.setUniformMat4("u_MVP", m_proj * view);
}

void Renderer::zoom(float value)
{
	m_zoom = value;
	resize(m_width, m_height);
}

std::array<Vertex, 4> Renderer::createQuad(float x, float y)
{
	float size = 20.f;

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
