#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <random>
#include <glm/glm.hpp>

struct Cell {
    glm::ivec3 corners[8];
    glm::vec3 directions[8];
    float cos_distance[8];
};

class PerlinNoise
{
public:
    PerlinNoise(int seed);
    glm::vec3 randomVector(glm::ivec3 v);
    glm::vec3 normal(glm::vec3 v);
    float noise(glm::vec3 v);

    Cell constructCell(glm::ivec3 innerCorner, glm::ivec3 outerCorner, glm::vec3 v);

private:
    std::mt19937 seed_generator;
    std::uniform_real_distribution<float> distr;
    std::mt19937 rand;
    std::mt19937::result_type seed_x;
    std::mt19937::result_type seed_y;
    std::mt19937::result_type seed_z;

    float lerp(float a, float b, float x);
    float fade(float t);
};

#endif // PERLINNOISE_H
