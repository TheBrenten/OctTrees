#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"



ShaderProgramSource Shader::parseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type,
	const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::createShader(const std::string& vertexShader,
	const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	return program;
}



int Shader::getUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}

Shader::Shader(const std::string& filepath) 
	: m_FilePath(filepath), m_RendererID(0) {
	ShaderProgramSource source = parseShader(filepath);
	m_RendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int i0) {
	GLCall(glUniform1i(getUniformLocation(name), i0));
}

void Shader::setUniform1f(const std::string& name, float f0) {
	GLCall(glUniform1f(getUniformLocation(name), f0));
}

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3) {
	GLCall(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
}

void Shader::setUniformMat4f(const std::string& name, glm::mat4 matrix) {
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix[0][0]));
}
