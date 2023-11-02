#pragma once

#include <string>
#include <unordered_map>
#include "Utils/glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform1f(const std::string& name, float v0);
	void setUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	struct ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

	int getUniformLocation(const std::string& name);
	
	ShaderProgramSource parseShader();
	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned int m_id;
	std::string m_filepath;
	std::unordered_map<std::string, int> m_cachedUniformLocations;
};