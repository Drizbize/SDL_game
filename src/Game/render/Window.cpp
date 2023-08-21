#include "Window.h"
#include "SDL.h"
#include "glad/glad.h"
#include "Listeners/WindowListener.h"
#include "Game.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>

#include "Utils/Utils.h"

int WindSizeX = 0;
int WindSizeY = 0;

enum MoveKeys
{
	Up = SDL_SCANCODE_W,
	Down = SDL_SCANCODE_S,
	Left = SDL_SCANCODE_A,
	Right = SDL_SCANCODE_D
};

struct Vector2
{
	float x = 0;
	float y = 0;
};

struct Particle
{
	std::string Name = "";
	float posX;
	float posY;

	float velocityX = 0.0f;
	float velocityY = 0.0f;

	game::MYColors color;
};

struct ParticleType
{
	std::string ColorNameType;
	std::vector<Particle> Particles;
};

struct PartTypeParam
{
	float Gravity;
	int GravityRadius;
};

void RandPosition(Particle &Par)
{
	Par.posX = rand() % (WindSizeX * 2) - WindSizeX;
	Par.posY = rand() % (WindSizeY * 2) - WindSizeY;
}

float RandGravity()
{
	return (float)((rand() % 2000 - 1000) * 0.001);
}

int RandRadius()
{
	return rand() % 200 + 75;
}


void CreateParticles(std::string Name, int Amount, game::MYColors color, std::vector<ParticleType> &SaveTo)
{
	ParticleType NewParticleTypeTable;
	NewParticleTypeTable.ColorNameType = Name;
	for (int i = 0; i < Amount; i++)
	{
		Particle NewParticle;
		NewParticle.Name = Name + std::to_string(i);
		RandPosition(NewParticle);
		NewParticle.color = color;

		NewParticleTypeTable.Particles.push_back(NewParticle);
	}

	SaveTo.push_back(NewParticleTypeTable);
}

ParticleType& FindTypeTableByColorType(std::vector<ParticleType>& ParticleTable, std::string &NameType)
{
	for (ParticleType &Type : ParticleTable)
	{
		if (Type.ColorNameType == NameType)
		{
			return Type;
		}
	}

	return ParticleTable.front();
}

void Rule(std::vector<ParticleType> &ParticleTable, std::string &ParticleName1, std::string &ParticleName2, float Gravity, int &Radius)
{
	ParticleType &ParticleType1 = FindTypeTableByColorType(ParticleTable, ParticleName1);
	ParticleType &ParticleType2 = FindTypeTableByColorType(ParticleTable, ParticleName2);

	for (Particle &Part1 : ParticleType1.Particles)
	{
		Vector2 ForceVector{ 0.0f, 0.0f };

		for (Particle &Part2 : ParticleType2.Particles)
		{
			float dx = Part1.posX - Part2.posX;
			float dy = Part1.posY - Part2.posY;
			float Distance = sqrt(dx*dx + dy*dy);
			if (Distance > 0 && Distance < Radius)
			{
				float Force = Gravity * (1 / Distance);

				ForceVector.x += Force * dx;
				ForceVector.y += Force * dy;
			}
		}

		Part1.velocityX = (Part1.velocityX + ForceVector.x) * 0.5f;
		Part1.velocityY = (Part1.velocityY + ForceVector.y) * 0.5f;
		
		Part1.posX += Part1.velocityX;
		Part1.posY += Part1.velocityY;

		/*if (Part1.posX < -WindSizeX)
		{
			Part1.posX = -WindSizeX;
		}
		else if (Part1.posX > WindSizeX)
		{
			Part1.posX = WindSizeX;
		}

		if (Part1.posY < -WindSizeY)
		{
			Part1.posY = -WindSizeY;
		}
		else if (Part1.posY > WindSizeY)
		{
			Part1.posY = WindSizeY;
		}*/

		if (abs(Part1.posX) > WindSizeX + 10 || abs(Part1.posY) > WindSizeY + 10)
		{
			RandPosition(Part1);
		}
		else if (abs(Part1.posX) > WindSizeX || abs(Part1.posY) > WindSizeY)
		{
			Part1.velocityX *= -1.0f;
			Part1.velocityY *= -1.0f;
		}
	}
}

namespace game
{
	struct Window::Impl
	{
		SDL_Window* m_window;
		SDL_GLContext m_context;
	};


Window::Window(int id, WindowListener* listener)
	: m_id(id)
	, m_listener(listener)
	, m_width(1000)
	, m_height(700)
{
	m_opengl = std::make_unique<OpenGl>(m_width, m_height);

	WindSizeX = m_width;
	WindSizeY = m_height;

	// this window has a strong SDL && OpenGL dependancy,
	// it is better to have a generic one so we can update it using other API
	// TODO: change Window into interface
	m_impl = std::make_unique<Impl>();

	SDL_Init(SDL_INIT_VIDEO);

	m_impl->m_window = SDL_CreateWindow(
		"Game window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_width,
		m_height,
		SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	
	m_impl->m_context = SDL_GL_CreateContext(m_impl->m_window);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}

Window::~Window()
{
	SDL_DestroyWindow(m_impl->m_window);
	SDL_Quit();
}

void Window::run()
{
	srand(time(0));

	SDL_Event event;
	bool running = true;
	
	std::vector<ParticleType> AllParticles;
	
	CreateParticles("Red", 325, MYColors::Red, AllParticles);
	CreateParticles("Blue", 325, MYColors::Blue, AllParticles);
	CreateParticles("Green", 325, MYColors::Green, AllParticles);
	CreateParticles("White", 325, MYColors::White, AllParticles);
	CreateParticles("Yellow", 325, MYColors::Yellow, AllParticles);
	//CreateParticles("Pink", 250, MYColors::Pink, AllParticles);
	//CreateParticles("Cyan", 250, MYColors::Cyan, AllParticles);

	std::vector<std::vector<PartTypeParam>> GravityRules;
	for (ParticleType &Type : AllParticles)
	{
		std::vector<PartTypeParam> TypeGravityRole;
		for (ParticleType& Type2 : AllParticles)
		{
			PartTypeParam Param;
			Param.Gravity = RandGravity();
			Param.GravityRadius = RandRadius();
			TypeGravityRole.push_back(Param);
		}
		GravityRules.push_back(TypeGravityRole);

		for (Particle &Part : Type.Particles)
		{
			m_opengl->initPlayerOpengl(Part.Name);
			m_opengl->setColor(Part.Name, Part.color);
		}
	}

	Uint32 prevTime = SDL_GetTicks();

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				SDL_KeyboardEvent key = event.key;

				switch (event.key.keysym.scancode)
				{
				case MoveKeys::Up:
					break;
				case MoveKeys::Down:
					break;
				case MoveKeys::Left:
					break;
				case MoveKeys::Right:
					break;
				case SDL_SCANCODE_Q:
					break;
				case SDL_SCANCODE_R:
					for (ParticleType& Type : AllParticles)
					{
						for (Particle& Part : Type.Particles)
						{
							RandPosition(Part);
						}
					}
					break;
				case SDL_SCANCODE_SPACE:
					GravityRules.clear();
					for (ParticleType& Type : AllParticles)
					{
						std::vector<PartTypeParam> TypeGravityRole;
						for (ParticleType& Type2 : AllParticles)
						{
							PartTypeParam Param;
							Param.Gravity = RandGravity();
							Param.GravityRadius = RandRadius();
							TypeGravityRole.push_back(Param);
						}
						GravityRules.push_back(TypeGravityRole);
					}
					break;
				}
				break;
			case SDL_KEYUP:
				//m_listener->onKeyReleased(event.key.keysym.scancode);
				break;
			case SDL_TEXTINPUT:
				break;
			}
		}

		int WindSizeX = m_height;

		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - prevTime) / 25.0f;
		prevTime = currentTime;

		for (int iType1 = 0; iType1 < AllParticles.size(); iType1++)
		{
			ParticleType& Type1 = AllParticles[iType1];
			for (int iType2 = 0; iType2 < AllParticles.size(); iType2++)
			{
				ParticleType& Type2 = AllParticles[iType2];
				PartTypeParam &Params = GravityRules[iType1][iType2];

				Rule(AllParticles, Type1.ColorNameType, Type2.ColorNameType, Params.Gravity * deltaTime, Params.GravityRadius);
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (ParticleType &Type : AllParticles)
		{
			for (Particle &Part : Type.Particles)
			{
				m_opengl->updatePlayerVetrtices(Part.Name, Part.posX, Part.posY, 5.0f, 5.0f);
				m_opengl->drawShader(Part.Name);
			}
		}
		
		//int x = 0;
		//for (auto obj : m_objects)
		//{
		//	std::string name = "kavo" + std::to_string(x);
		//	x++;
		//	m_opengl->updatePlayerVetrtices(name, obj.posX, obj.posY, obj.width, obj.height);
		//	m_opengl->drawShader(name);
		//}

		SDL_GL_SwapWindow(m_impl->m_window);
	}
}

}