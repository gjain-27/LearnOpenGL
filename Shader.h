#pragma once
#include <glad/glad.h>
#include <string>

class Shader {
private:
    unsigned int ID;
    std::string loadShaderSource(const char* path);

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
};