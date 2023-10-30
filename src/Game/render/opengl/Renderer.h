#pragma once

#include <array>
#include <vector>
#include "VertexArray.h"
#include "Shader.h"

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

	void init();

	void addObject(float x, float y);
	void draw() const;

private:
	std::array<Vertex, 4> createQuad(float x, float y);

	mutable std::vector<Vertex> m_vertexes;
	mutable unsigned int m_offset;
	unsigned int m_capacity;
	mutable unsigned int m_indicesCount;

	VertexArray m_va;
	VertexBuffer m_vb;
	VertexBufferLayout m_layout;
	Shader m_shader;
};