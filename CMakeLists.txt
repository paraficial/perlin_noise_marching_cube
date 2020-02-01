cmake_minimum_required(VERSION 2.8)

project(perlin_noise_pointclouds)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

set(HEADERS
    src/data.h
    src/mesh.h
    src/visualization.h
    src/shader.h
    src/perlinnoise.h)

set(SOURCES
    src/main.cpp
    src/data.cpp
    src/mesh.cpp
    src/visualization.cpp
    src/perlinnoise.cpp)

set(SHADERS
    shaders/shader.vert
    shaders/shader.frag)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLM_LIBRARIES})
