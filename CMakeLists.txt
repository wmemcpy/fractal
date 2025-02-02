cmake_minimum_required(VERSION 3.10)
project(FractalRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    lib/glad/include
    lib/glm
    lib/glfw/include
    lib/imgui
    include
)

add_executable(FractalRenderer
    src/main.cpp
    src/shader.cpp
    src/state.cpp
    src/renderer.cpp
    src/utils.cpp
    src/presets.cpp
    lib/glad/src/glad.c
)

add_library(imgui STATIC
    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC lib/imgui lib/imgui/backends)
target_link_libraries(FractalRenderer imgui)

target_link_libraries(FractalRenderer
    ${OPENGL_LIBRARIES}
    glfw
)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

set_target_properties(FractalRenderer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
