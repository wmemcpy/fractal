#ifndef STATE_HPP
#define STATE_HPP

#include <glm/glm.hpp>

enum class FractalType
{
    Julia,
    Mandelbrot,
    BurningShip,
    Celtic
};

struct State
{
    float zoom = 1.0f;
    glm::vec2 offset{0.0f};
    glm::vec2 params{-0.4f, 0.6f};
    int maxIterations = 100;
    bool autoRotate = false;
    float time = 0.0f;
    float rotationSpeed = 1.0f;
    float rotationRadius = 0.7f;
    glm::vec3 color1{0.0f, 0.7f, 1.0f}; // Bleu
    glm::vec3 color2{0.9f, 0.9f, 0.0f}; // Jaune
    glm::vec3 color3{0.0f, 0.0f, 0.0f}; // Noir
    float colorOffset = 0.0f;
    float colorScale = 1.0f;
    FractalType fractalType = FractalType::Julia;
};

#endif
