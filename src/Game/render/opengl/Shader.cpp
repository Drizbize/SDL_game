#include "Shader.h"

#include "glad/glad.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_id(0)
{
	ShaderProgramSource source = parseShader();
	m_id = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::bind() const
{
	glUseProgram(m_id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	const int location = getUniformLocation(name);
	if(location != -1)
		glUniform4f(location, v0, v1, v2, v3);
}

void Shader::setUniform1f(const std::string& name, float v0)
{
	const int location = getUniformLocation(name);
	if (location != -1)
		glUniform1f(location, v0);
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_cachedUniformLocations.find(name) == m_cachedUniformLocations.end())
		m_cachedUniformLocations[name] = glGetUniformLocation(m_id, name.c_str());

	return m_cachedUniformLocations[name];
}

Shader::ShaderProgramSource Shader::parseShader()
{
	std::ifstream stream(m_filepath);

	enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::None;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::Fragment;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';
	std::cout << "ss[0] = " << ss[0].str() << "\n";
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertext" : "Fragment") << " Shader : " << message << "\n";
	}

	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteProgram(vs);
	glDeleteProgram(fs);
	
	return program;
}
