#pragma once

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

    // Program object ID
    unsigned int ID;

    // Reading shader code, constructing it, compiling and linking  
    Shader(const GLuint ID, const GLchar* vertexPath, const GLchar* fragmentPath);

    // Shader activation
	void use();

    // Cleanup
    void cleanup();

    // Manipulating uniforms  
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};

