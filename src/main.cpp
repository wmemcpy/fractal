#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "state.hpp"
#include "renderer.hpp"
#include "shader.hpp"

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Erreur lors de l'initialisation de GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Julia Set Explorer", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Erreur lors de la création de la fenêtre" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Erreur lors de l'initialisation de GLAD" << std::endl;
        return -1;
    }

    State state;
    Renderer renderer(window, state);

    glfwSetWindowUserPointer(window, &state);
    glfwSetScrollCallback(window, Renderer::scrollCallback);

#define SHADER_PATH_VERTEX "./shaders/vertex.glsl"
#define SHADER_PATH_FRAGMENT "./shaders/fragment.glsl"

    Shader shader(SHADER_PATH_VERTEX, SHADER_PATH_FRAGMENT);

    renderer.initImGui();
    renderer.initBuffers();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        renderer.update(shader);
        renderer.render(shader);
        glfwSwapBuffers(window);
    }

    renderer.cleanup();
    glfwTerminate();
    return 0;
}
