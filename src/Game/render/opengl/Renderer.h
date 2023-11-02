#pragma once

#include <array>
#include <vector>
#include "VertexArray.h"
#include "Shader.h"

#include "Utils/glm/glm.hpp"
#include "Utils/glm/gtc/matrix_transform.hpp"

class IndexBuffer;
class Shader;

struct Vec4
{
	float x, y, z, w;
};

struct Vec3
{
	float x, y, z;
};

struct Vec2
{
	float x, y;
};

struct Vertex
{
	Vec3 position;
	//Vec4 color;
};

class Renderer
{
public:
	Renderer(const std::string& filepath);

	void init(float width, float height);

	void addObject(float x, float y);
	void draw() const;

	void moveCamera(int x, int y);
	void resize(int width, int height);
	void zoom(float value);

private:
	std::array<Vertex, 4> createQuad(float x, float y);

	mutable std::vector<Vertex> m_vertexes;
	mutable std::vector<unsigned int> m_indices;
	mutable unsigned int m_offset;
	unsigned int m_capacity;
	mutable unsigned int m_indicesCount;

	VertexArray m_va;
	VertexBuffer m_vb;
	VertexBufferLayout m_layout;
	Shader m_shader;

	glm::mat4 m_proj;
	glm::vec3 m_view;

	float m_width;
	float m_height;
	float m_zoom;
};