#include "Engine/Shader.h"
#include "Helpers/ShaderLoader.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLuint ID, const GLchar* vertexPath, const GLchar* fragmentPath)
{
	this->ID = ID;

	// Get vertex shader source
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = loadShaderFromFile(vertexPath);

	// Get fragment shader source
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSource = loadShaderFromFile(fragmentPath);

	const char* vs_str = vertexShaderSource.c_str();
	const char* fs_str = fragmentShaderSource.c_str();

	glShaderSource(vertexShader, 1, &vs_str, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &fs_str, NULL);
	glCompileShader(fragmentShader);

	GLint successv, successf, successl;
	GLchar infoLogv[512], infoLogf[512], infoLogl[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successv);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successf);

	if (!successv)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLogv);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogv << std::endl;
	}

	if (!successf)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLogf);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogf << std::endl;
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	glLinkProgram(this->ID);

	glGetProgramiv(this->ID, GL_LINK_STATUS, &successl);
	if (!successl) {
		glGetProgramInfoLog(this->ID, 512, NULL, infoLogl);
	}

	// Cleanup

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::cleanup()
{
	glDeleteProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVector3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVector4(const std::string& name, glm::vec4 value) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4Array(const std::string& name, const std::vector<glm::mat4>& values) const
{
	const GLint location = glGetUniformLocation(ID, name.c_str());
	for (size_t i = 0; i < values.size(); i++)
	{
		glUniformMatrix4fv(location + i, 1, GL_FALSE, glm::value_ptr(values[i]));
	}
}

void Shader::setSkinningMatrices(const glm::mat4* bones) const
{
	const GLint blockIndex = glGetUniformBlockIndex(ID, "skinningBuffer");
	glUniformBlockBinding(ID, blockIndex, 0); // Assuming binding point is 0
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 512, bones, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Set the entire array at once
	glUniformMatrix4fv(glGetUniformLocation(ID, "skin.bones"), 512, GL_FALSE, glm::value_ptr(bones[0]));
}

