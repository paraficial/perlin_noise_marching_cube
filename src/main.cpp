#include <glm/glm.hpp>
#include "shader.h"
#include "mesh.h"
#include "data.h"
#include "visualization.h"
#include "perlinnoise.h"

int main()
{
    PerlinNoise perlin(20);
    Mesh mesh;
    Data data(&perlin, 32, 3.1f, &mesh, 0.0f);
    mesh.exportOBJ("./test.obj");

    // visualization
    Visualization visualization(800, 800);
    Shader shader("../perlin_noise_pointclouds/shaders/shader.vert",
                  "../perlin_noise_pointclouds/shaders/shader.frag");
    visualization.loop(&mesh, &shader, glm::radians(225.0f));
    return 0;
}
