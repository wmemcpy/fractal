#include "utils.hpp"

glm::vec3 hsv2rgb(glm::vec3 c)
{
    glm::vec4 K = glm::vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(c.x) + glm::vec3(K.x, K.y, K.z)) * 6.0f - glm::vec3(K.w));
    return c.z * glm::mix(glm::vec3(K.x), glm::clamp(p - glm::vec3(K.x), 0.0f, 1.0f), c.y);
}
