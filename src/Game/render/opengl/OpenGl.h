#pragma once
#include <map>
#include <string>

namespace game
{
	struct GlState
	{
		unsigned int VBO;
		unsigned int VAO;
		unsigned int vertexShader;
		unsigned int fragmentShader;
		int shader;
		float vertices[24];
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
	};

	enum class MYColors
	{
		Red = 1,
		Green,
		Blue,
		White,
		Yellow,
		Pink,
		Cyan
	};

	class OpenGl
	{
	public:
		OpenGl(int width, int height);
		~OpenGl() = default;

		//bool Init();
		bool initPlayerOpengl(const std::string& name);
		bool updatePlayerVetrtices(const std::string& name, float posX, float posY, float width, float height);
		bool drawShader(const std::string& name);
		bool setColor(const std::string& name, MYColors colors);

		//GLuint vertexVBO;
	private:
		std::map<std::string, GlState> m_states;

		int m_width;
		int m_height;
	};
}