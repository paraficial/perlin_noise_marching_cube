#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Mesh;
class Shader;
class Visualization
{
public:
    Visualization(int width, int height);
    int init(int width, int height);
    void loop(Mesh *mesh, Shader *shader, float angle);
private:
    SDL_Window *window;
    SDL_GLContext context;

    glm::mat4 projection, view, model, mvp;
    float m_angle = 0.0f;
};

#endif // VISUALIZATION_H
