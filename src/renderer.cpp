#include "renderer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "presets.hpp"
#include "utils.hpp"
#include <iostream>

Renderer::Renderer(GLFWwindow *window, State &state) : window(window), state(state) {}

Renderer::~Renderer() {}

void Renderer::initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
}

void Renderer::initBuffers()
{
    float vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::update(Shader &shader)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Julia Set Explorer", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Navigation:");
        ImGui::BulletText("Click and drag to move");
        ImGui::BulletText("Scroll wheel to zoom");
        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderInt("Iterations", &state.maxIterations, 1, 500);
        ImGui::SliderFloat("Zoom", &state.zoom, 0.1f, 10000.0f);
        if (state.fractalType == FractalType::Julia)
        {
            ImGui::SliderFloat2("Parameters (c)", &state.params.x, -2.0f, 2.0f);
        }
        ImGui::Separator();

        ImGui::Text("Fractal Type:");
        if (ImGui::RadioButton("Julia", state.fractalType == FractalType::Julia))
        {
            state.fractalType = FractalType::Julia;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Mandelbrot", state.fractalType == FractalType::Mandelbrot))
        {
            state.fractalType = FractalType::Mandelbrot;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Burning Ship", state.fractalType == FractalType::BurningShip))
        {
            state.fractalType = FractalType::BurningShip;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Celtic", state.fractalType == FractalType::Celtic))
        {
            state.fractalType = FractalType::Celtic;
        }
        ImGui::Separator();

        ImGui::Text("Colors:");
        ImGui::ColorEdit3("Color 1", (float *)&state.color1);
        ImGui::ColorEdit3("Color 2", (float *)&state.color2);
        ImGui::ColorEdit3("Color 3", (float *)&state.color3);
        ImGui::SliderFloat("Color Offset", &state.colorOffset, 0.0f, 100.0f);
        ImGui::SliderFloat("Color Scale", &state.colorScale, 1.0f, 100.0f);
    }

    if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Auto Rotate", &state.autoRotate);
        if (state.autoRotate)
        {
            ImGui::SliderFloat("Speed", &state.rotationSpeed, 0.001f, 0.2f);
            ImGui::SliderFloat("Radius", &state.rotationRadius, 0.1f, 1.0f);
        }
    }

    if (ImGui::CollapsingHeader("Presets", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (int i = 0; presets[i].name != nullptr; i++)
        {
            if (ImGui::Button(presets[i].name))
            {
                state.params = presets[i].params;
            }
        }
    }

    if (ImGui::Button("Reset View"))
    {
        state.zoom = 1.0f;
        state.offset = glm::vec2(0.0f);
    }

    ImGui::End();

    if (state.autoRotate && state.fractalType == FractalType::Julia)
    {
        state.time += 0.016f * state.rotationSpeed;
        state.params.x = state.rotationRadius * sin(state.time);
        state.params.y = state.rotationRadius * cos(state.time * 0.6f);
    }

    if (ImGui::IsMouseClicked(0) && !ImGui::GetIO().WantCaptureMouse)
    {
        isDragging = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastPos = glm::vec2(xpos, ypos);
    }

    if (ImGui::IsMouseReleased(0))
    {
        isDragging = false;
    }

    if (isDragging)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 currentPos(xpos, ypos);
        glm::vec2 delta = currentPos - lastPos;
        state.offset -= glm::vec2(delta.x, -delta.y) * 0.003f / state.zoom;
        lastPos = currentPos;
    }
}

void Renderer::render(Shader &shader)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fond noir
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    glUniform2f(glGetUniformLocation(shader.getID(), "u_resolution"), width, height);
    glUniform1i(glGetUniformLocation(shader.getID(), "u_maxIterations"), state.maxIterations);
    glUniform2f(glGetUniformLocation(shader.getID(), "u_offset"), state.offset.x, state.offset.y);
    glUniform1f(glGetUniformLocation(shader.getID(), "u_zoom"), state.zoom);
    glUniform2f(glGetUniformLocation(shader.getID(), "u_params"), state.params.x, state.params.y);
    glUniform3f(glGetUniformLocation(shader.getID(), "u_color1"), state.color1.x, state.color1.y, state.color1.z);
    glUniform3f(glGetUniformLocation(shader.getID(), "u_color2"), state.color2.x, state.color2.y, state.color2.z);
    glUniform3f(glGetUniformLocation(shader.getID(), "u_color3"), state.color3.x, state.color3.y, state.color3.z);
    glUniform1f(glGetUniformLocation(shader.getID(), "u_colorOffset"), state.colorOffset);
    glUniform1f(glGetUniformLocation(shader.getID(), "u_colorScale"), state.colorScale);
    glUniform1i(glGetUniformLocation(shader.getID(), "u_fractalType"), static_cast<int>(state.fractalType));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    State *state = static_cast<State *>(glfwGetWindowUserPointer(window));
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        glm::vec2 mousePos{
            (xpos / width) * 2.0f - 1.0f,
            -((ypos / height) * 2.0f - 1.0f)};

        glm::vec2 oldWorldPos = mousePos / state->zoom + state->offset;

        float zoomFactor = (yoffset > 0) ? 1.1f : 0.9f;
        state->zoom *= zoomFactor;

        glm::vec2 newWorldPos = mousePos / state->zoom + state->offset;

        state->offset += oldWorldPos - newWorldPos;
    }
}
