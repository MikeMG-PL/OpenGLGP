#pragma once

#include "glad/glad.h"
#include <string>
#include <sstream>
#include "glm/glm.hpp"

class Shader
{
public:

    // Program object ID
    unsigned int ID;

    // Reading shader code, constructing it, compiling and linking  
    Shader(const GLuint ID, const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader() = default;

    // Shader activation
	void use();

    // Cleanup
    void cleanup();

    // Setters for uniforms
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVector3(const std::string& name, glm::vec3 value) const;
    void setVector4(const std::string& name, glm::vec4 value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
};

