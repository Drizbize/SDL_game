#include "OpenGl.h"

#include "glad/glad.h"
#include "Utils/Utils.h"

namespace game
{
	const char* vertexShaderSource = R"(
    #version 330 core
	layout (location = 0) in vec3 inPosition;
	layout (location = 1) in vec3 inColor;
	out vec4 FragColor;

    void main()
    {
        gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
		FragColor = vec4(inColor.x, inColor.y, inColor.z, 1.0);
    }
)";

	const char* fragmentShaderSource = R"(
    #version 330 core
	in vec4 FragColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(FragColor.x, FragColor.y, FragColor.z, 1.0);
    }
)";

	OpenGl::OpenGl(int width, int height)
		: m_width(width)
		, m_height(height)
	{
	}

	bool OpenGl::initPlayerOpengl(const std::string& name)
	{
		auto it = m_states.find(name);
		if (it != m_states.end())
			return false;

		GlState state{};
		m_states[name] = state;
		
		glGenVertexArrays(1, &m_states[name].VAO);
		glGenBuffers(1, &m_states[name].VBO);

		glBindVertexArray(m_states[name].VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_states[name].VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_states[name].vertices), m_states[name].vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_states[name].vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_states[name].vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(m_states[name].vertexShader);

		m_states[name].fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_states[name].fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(m_states[name].fragmentShader);

		m_states[name].shader = glCreateProgram();
		glAttachShader(m_states[name].shader, m_states[name].vertexShader);
		glAttachShader(m_states[name].shader, m_states[name].fragmentShader);
		glLinkProgram(m_states[name].shader);
		
		return true;
	}

	bool OpenGl::updatePlayerVetrtices(const std::string& name, float posX, float posY, float width, float height)
	{
		auto it = m_states.find(name);
		if (it == m_states.end())
			return false;

		float topLeftX = (posX - width / 2) / m_width;
		float topLeftY = (posY - height / 2) / m_height;
		float topRightX = (posX + width / 2) / m_width;
		float topRightY = topLeftY;
		float botLeftX = topLeftX;
		float botLeftY = (posY + height / 2) / m_height;
		float botRightX = topRightX;
		float botRightY = botLeftY;

		Utils::clamp(topLeftX, -1.f, 1.f);
		Utils::clamp(topLeftY, -1.f, 1.f);
		Utils::clamp(topRightX, -1.f, 1.f);
		Utils::clamp(topRightY, -1.f, 1.f);
		Utils::clamp(botLeftX, -1.f, 1.f);
		Utils::clamp(botLeftY, -1.f, 1.f);
		Utils::clamp(botRightX, -1.f, 1.f);
		Utils::clamp(botRightY, -1.f, 1.f);

		float vertices[] = {
			// positions					// colors
			 botRightX, botRightY,	0.0f,  m_states[name].r, m_states[name].g, m_states[name].b,   // bottom right
			 botLeftX,	botLeftY,	0.0f,  m_states[name].r, m_states[name].g, m_states[name].b,   // bottom left
			 topLeftX,  topLeftY,	0.0f,  m_states[name].r, m_states[name].g, m_states[name].b,   // top left
			 topRightX, topRightY,	0.0f,	m_states[name].r, m_states[name].g, m_states[name].b   // top right
		};
		for(int i = 0; i < 24; i++)
			m_states[name].vertices[i] = vertices[i];
		return true;
	}
	bool OpenGl::setColor(const std::string& name, MYColors colors)
	{
		auto it = m_states.find(name);
		if (it == m_states.end())
			return false;

		m_states[name].r = 0.f;
		m_states[name].g = 0.f;
		m_states[name].b = 0.f;
		switch (colors)
		{
		case MYColors::Red:
			m_states[name].r = 1.f;
			break;

		case MYColors::Blue:
			m_states[name].b = 1.f;
			break;

		case MYColors::Green:
			m_states[name].g = 1.f;
			break;

		}

		return true;
	}
	bool OpenGl::drawShader(const std::string& name)
	{
		auto it = m_states.find(name);
		if (it == m_states.end())
			return false;


		glGenVertexArrays(1, &m_states[name].VAO);
		glGenBuffers(1, &m_states[name].VBO);

		glBindVertexArray(m_states[name].VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_states[name].VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_states[name].vertices), m_states[name].vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glUseProgram(m_states[name].shader);
		glBindVertexArray(m_states[name].VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
		return true;
	}
}