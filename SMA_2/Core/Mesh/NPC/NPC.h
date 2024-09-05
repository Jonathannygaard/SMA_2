#pragma once
#include "../Mesh.h"

class NPC : Mesh
{
public:
    std::vector<glm::vec3> Points;

    int i = 0;

    float cubeMovement = 0.005f;

    void initNPC();

    glm::vec4 interpolation(std::vector<glm::vec3> points);

    float GetZ(glm::vec4 solution, float x);

    void updatePos(Cube& cube);
};
