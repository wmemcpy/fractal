#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>

class Shader
{
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void use();
    GLuint getID() const { return programID; }

private:
    GLuint programID;
    GLuint createShader(const std::string &source, GLenum type);
};

#endif
