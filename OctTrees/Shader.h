#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"


struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	// caching for uniforms
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int getUniformLocation(const std::string& name);
public:
	Shader(const std::string& filepath);
	~Shader();
	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform1i(const std::string& name, int i0);
	void setUniform1f(const std::string& name, float f0);
	void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void setUniformMat4f(const std::string& name, glm::mat4 matrix);
};

