#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "state.hpp"
#include "shader.hpp"

class Renderer
{
public:
    Renderer(GLFWwindow *window, State &state);
    ~Renderer();

    void initImGui();
    void initBuffers();
    void update(Shader &shader);
    void render(Shader &shader);
    void cleanup();
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

private:
    GLFWwindow *window;
    State &state;
    GLuint VAO, VBO, EBO;
    bool isDragging = false;
    glm::vec2 lastPos;
};

#endif
