#ifndef PRESETS_Hpp
#define PRESETS_Hpp

#include <glm/glm.hpp>

struct JuliaParams
{
    glm::vec2 params;
    const char *name;
};

extern const JuliaParams presets[];

#endif
