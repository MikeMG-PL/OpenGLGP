#include "ShaderLoader.h"

#include <fstream>
#include <sstream>

std::string loadShaderFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
