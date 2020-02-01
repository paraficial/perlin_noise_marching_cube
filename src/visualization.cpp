#include "visualization.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "shader.h"


Visualization::Visualization(int width, int height)
{
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0, 0, 6), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    model = glm::mat4(1.0f);
    init(width, height);
}

int Visualization::init(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to init SDL" << std::endl;
        return -1;
    }

    window = SDL_CreateWindow(
                "Perlin Noise",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width,
                height,
                SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return 0;
}

void Visualization::loop(Mesh *mesh, Shader *shader, float angle)
{
    mesh->gpu();
    bool loop = true;
    while (loop) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                loop = false;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    loop = false;
            }
        }

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_angle += 0.01f;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(0, 1, 0));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0));
        model = rotation * scale * translation;

        shader->setMatrix("projection", projection);
        shader->setMatrix("view", view);
        shader->setMatrix("model", model);
        mesh->draw(shader);

        SDL_GL_SwapWindow(window);
    }
}
