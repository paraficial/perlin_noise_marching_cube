#include "data.h"
#include <glm/glm.hpp>
#include <iostream>

#include "perlinnoise.h"
#include "mesh.h"

Data::Data(PerlinNoise *perlin, unsigned int resolution, float zoom, Mesh *mesh, float isolevel) :
    perlin(perlin),
    resolution(resolution),
    zoom(zoom),
    mesh(mesh)
{
    this->volume = new float[resolution * resolution * resolution];
    std::cout << "start generating perlin volume" << std::endl;
    this->generatePerlinVolume();
    std::cout << "start triangulating perlin volume" << std::endl;
    this->triangulateVolume(isolevel);
}

void Data::generatePerlinVolume()
{
    unsigned int i;
    for (unsigned int x = 0; x < resolution; x++) {
        for (unsigned int y = 0; y < resolution; y++) {
            for (unsigned int z = 0; z < resolution; z++) {
                i = x * resolution * resolution + y * resolution + z;
                glm::vec3 v = glm::vec3(x, y, z) * (zoom / resolution);
                float value = perlin->noise(v);
                volume[i] = value;
            }
        }
    }
}

int Data::triangulateVolume(float isolevel)
{
    for (unsigned int x = 0; x < resolution - 1; x++) {
        for (unsigned int y = 0; y < resolution - 1; y++) {
            for (unsigned int z = 0; z < resolution - 1; z++) {
                GridCell cell;
                cell.corners[0] = glm::uvec3(x + 1, y, z);
                cell.corners[1] = glm::uvec3(x, y, z);
                cell.corners[2] = glm::uvec3(x, y + 1, z);
                cell.corners[3] = glm::uvec3(x + 1, y + 1, z);
                cell.corners[4] = glm::uvec3(x + 1, y, z + 1);
                cell.corners[5] = glm::uvec3(x, y, z + 1);
                cell.corners[6] = glm::uvec3(x, y + 1, z + 1);
                cell.corners[7] = glm::uvec3(x + 1, y + 1, z + 1);

                cell.cornerValues[0] = this->getVolumeValue(cell.corners[0]);
                cell.cornerValues[1] = this->getVolumeValue(cell.corners[1]);
                cell.cornerValues[2] = this->getVolumeValue(cell.corners[2]);
                cell.cornerValues[3] = this->getVolumeValue(cell.corners[3]);
                cell.cornerValues[4] = this->getVolumeValue(cell.corners[4]);
                cell.cornerValues[5] = this->getVolumeValue(cell.corners[5]);
                cell.cornerValues[6] = this->getVolumeValue(cell.corners[6]);
                cell.cornerValues[7] = this->getVolumeValue(cell.corners[7]);

                int cubeindex;
                cubeindex = 0;
                if (cell.cornerValues[0] < isolevel) cubeindex |= 1;
                if (cell.cornerValues[1] < isolevel) cubeindex |= 2;
                if (cell.cornerValues[2] < isolevel) cubeindex |= 4;
                if (cell.cornerValues[3] < isolevel) cubeindex |= 8;
                if (cell.cornerValues[4] < isolevel) cubeindex |= 16;
                if (cell.cornerValues[5] < isolevel) cubeindex |= 32;
                if (cell.cornerValues[6] < isolevel) cubeindex |= 64;
                if (cell.cornerValues[7] < isolevel) cubeindex |= 128;

                glm::vec3 vertlist[12];
                if (edgeTable[cubeindex] == 0)
                        continue;
                if (edgeTable[cubeindex] & 1)
                    vertlist[0] = vertexInterpolate(isolevel, cell.corners[0], cell.corners[1], cell.cornerValues[0], cell.cornerValues[1]);
                if (edgeTable[cubeindex] & 2)
                    vertlist[1] = vertexInterpolate(isolevel, cell.corners[1], cell.corners[2], cell.cornerValues[1], cell.cornerValues[2]);
                if (edgeTable[cubeindex] & 4)
                    vertlist[2] = vertexInterpolate(isolevel, cell.corners[2], cell.corners[3], cell.cornerValues[2], cell.cornerValues[3]);
                if (edgeTable[cubeindex] & 8)
                    vertlist[3] = vertexInterpolate(isolevel, cell.corners[3], cell.corners[0], cell.cornerValues[3], cell.cornerValues[0]);
                if (edgeTable[cubeindex] & 16)
                    vertlist[4] = vertexInterpolate(isolevel, cell.corners[4], cell.corners[5], cell.cornerValues[4], cell.cornerValues[5]);
                if (edgeTable[cubeindex] & 32)
                    vertlist[5] = vertexInterpolate(isolevel, cell.corners[5], cell.corners[6], cell.cornerValues[5], cell.cornerValues[6]);
                if (edgeTable[cubeindex] & 64)
                    vertlist[6] = vertexInterpolate(isolevel, cell.corners[6], cell.corners[7], cell.cornerValues[6], cell.cornerValues[7]);
                if (edgeTable[cubeindex] & 128)
                    vertlist[7] = vertexInterpolate(isolevel, cell.corners[7], cell.corners[4], cell.cornerValues[7], cell.cornerValues[4]);
                if (edgeTable[cubeindex] & 256)
                    vertlist[8] = vertexInterpolate(isolevel, cell.corners[0], cell.corners[4], cell.cornerValues[0], cell.cornerValues[4]);
                if (edgeTable[cubeindex] & 512)
                    vertlist[9] = vertexInterpolate(isolevel, cell.corners[1], cell.corners[5], cell.cornerValues[1], cell.cornerValues[5]);
                if (edgeTable[cubeindex] & 1024)
                    vertlist[10] = vertexInterpolate(isolevel, cell.corners[2], cell.corners[6], cell.cornerValues[2], cell.cornerValues[6]);
                if (edgeTable[cubeindex] & 2048)
                    vertlist[11] = vertexInterpolate(isolevel, cell.corners[3], cell.corners[7], cell.cornerValues[3], cell.cornerValues[7]);

                int ntriang = 0;
                Triangle triangles[6];
                for (int i = 0; triTable[cubeindex][i] != -1; i += 3) {
                    triangles[ntriang].vertices[0] = vertlist[triTable[cubeindex][i]] * (1.0f / (resolution + epsilon));
                    triangles[ntriang].vertices[1] = vertlist[triTable[cubeindex][i + 1]] * (1.0f / (resolution + epsilon));
                    triangles[ntriang].vertices[2] = vertlist[triTable[cubeindex][i + 2]] * (1.0f / (resolution + epsilon));
                    ntriang++;
                }

                if (ntriang == 0)
                    continue;

                for (int i = 0; i < ntriang; i++) {
                    unsigned int vhandle[3];
                    vhandle[0] = mesh->addVertex(triangles[i].vertices[0]);
                    vhandle[1] = mesh->addVertex(triangles[i].vertices[1]);
                    vhandle[2] = mesh->addVertex(triangles[i].vertices[2]);
                    mesh->addNormal(perlin->normal(triangles[i].vertices[0]));
                    mesh->addNormal(perlin->normal(triangles[i].vertices[1]));
                    mesh->addNormal(perlin->normal(triangles[i].vertices[2]));
                    mesh->addTriangle(vhandle[0], vhandle[1], vhandle[2]);
                }
            }
        }
    }
    std::cout << "vertices: " << mesh->vertices.size() << std::endl;
    std::cout << "normals: " << mesh->normals.size() << std::endl;
    std::cout << "indices: " << mesh->indices.size() << std::endl;
    return 0;
}

glm::vec3 Data::vertexInterpolate(float isolevel, glm::uvec3 corner1, glm::uvec3 corner2, float cornerValue1, float cornerValue2)
{
    assert((cornerValue1 < isolevel) || (cornerValue2 < isolevel));
    float factor = (isolevel - cornerValue1) / (cornerValue2 - cornerValue1);
    glm::vec3 result = glm::vec3(corner1) + factor * (glm::vec3(corner2) - glm::vec3(corner1));
    return result;
}

float Data::getVolumeValue(glm::uvec3 v) {
    return getVolumeValue(v.x, v.y, v.z);
}

float Data::getVolumeValue(unsigned int x, unsigned int y, unsigned int z)
{
    unsigned int i = x * resolution * resolution + y * resolution + z;
    assert((i >= 0) && (i < resolution * resolution * resolution));
    return this->volume[i];
}

float Data::getVolumeValue(unsigned int i)
{
    assert((i >= 0) && (i < resolution * resolution * resolution));
    return this->volume[i];
}
