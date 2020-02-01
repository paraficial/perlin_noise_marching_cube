#include "perlinnoise.h"
#include <iostream>

PerlinNoise::PerlinNoise(int seed)
{
    distr = std::uniform_real_distribution<float>(-1, 1);
    seed_generator.seed(static_cast<std::mt19937::result_type>(seed));
    seed_x = seed_generator();
    seed_y = seed_generator();
    seed_z = seed_generator();
}

glm::vec3 PerlinNoise::randomVector(glm::ivec3 v)
{
    glm::vec3 result = glm::vec3(1.0f);
    rand.seed(static_cast<unsigned long>(v.x) * seed_x + seed_x);
    result.x = distr(rand);
    rand.seed(static_cast<unsigned long>(v.y) * seed_y + seed_y);
    result.y = distr(rand);
    rand.seed(static_cast<unsigned long>(v.z) * seed_z + seed_z);
    result.z = distr(rand);

    return glm::normalize(result);
}

glm::vec3 PerlinNoise::normal(glm::vec3 v) {
    float h = 0.1f;
    float mx = noise(v + glm::vec3(h, 0, 0)) - noise(v - glm::vec3(h, 0, 0));
    float my = noise(v + glm::vec3(0, h, 0)) - noise(v - glm::vec3(0, h, 0));
    float mz = noise(v + glm::vec3(0, 0, h)) - noise(v - glm::vec3(0, 0, h));
    return glm::vec3(mx, my, mz) / 2.0f;
}

float PerlinNoise::noise(glm::vec3 v)
{
    glm::ivec3 innerCorner = static_cast<glm::ivec3>(v);
    glm::ivec3 outerCorner = static_cast<glm::ivec3>(v) + glm::ivec3(1);
    Cell cell = constructCell(innerCorner, outerCorner, v);

    glm::vec3 diffs = v - static_cast<glm::vec3>(innerCorner);

    float interp_hor_lf = lerp(cell.cos_distance[0], cell.cos_distance[4], fade(diffs.x));
    float interp_hor_uf = lerp(cell.cos_distance[1], cell.cos_distance[5], fade(diffs.x));
    float interp_hor_lb = lerp(cell.cos_distance[2], cell.cos_distance[6], fade(diffs.x));
    float interp_hor_ub = lerp(cell.cos_distance[3], cell.cos_distance[7], fade(diffs.x));
    float interp_ver_f = lerp(interp_hor_lf, interp_hor_uf, fade(diffs.y));
    float interp_ver_b = lerp(interp_hor_lb, interp_hor_ub, fade(diffs.y));
    float interpolated = lerp(interp_ver_f, interp_ver_b, fade(diffs.z));

    return interpolated;
}

Cell PerlinNoise::constructCell(glm::ivec3 innerCorner, glm::ivec3 outerCorner, glm::vec3 v)
{
    Cell cell;
    cell.corners[0] = glm::ivec3(innerCorner.x, innerCorner.y, innerCorner.z);
    cell.corners[1] = glm::ivec3(innerCorner.x, innerCorner.y, outerCorner.z);
    cell.corners[2] = glm::ivec3(innerCorner.x, outerCorner.y, innerCorner.z);
    cell.corners[3] = glm::ivec3(innerCorner.x, outerCorner.y, outerCorner.z);
    cell.corners[4] = glm::ivec3(outerCorner.x, innerCorner.y, innerCorner.z);
    cell.corners[5] = glm::ivec3(outerCorner.x, innerCorner.y, outerCorner.z);
    cell.corners[6] = glm::ivec3(outerCorner.x, outerCorner.y, innerCorner.z);
    cell.corners[7] = glm::ivec3(outerCorner.x, outerCorner.y, outerCorner.z);

    for (unsigned int i = 0; i < 8; i++) {
        cell.directions[i] = v - static_cast<glm::vec3>(cell.corners[i]);
        cell.cos_distance[i] = glm::dot(randomVector(cell.corners[i]), glm::normalize(cell.directions[i])); // normalize???
    }

    return cell;
}

float PerlinNoise::lerp(float a, float b, float x)
{
    return (1.0f - x) * a + x * b;
}

float PerlinNoise::fade(float t)
{
    double double_t = static_cast<double>(t);
    double result = 6 * pow(double_t, 5) - 15 * pow(double_t, 4) + 10 * pow(double_t, 3);
    return static_cast<float>(result);
}
