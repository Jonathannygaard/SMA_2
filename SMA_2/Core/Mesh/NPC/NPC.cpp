#define GLM_ENABLE_EXPERIMENTAL

#include "NPC.h"
#include "../../Application.h"
#include "glm/gtx/matrix_interpolation.hpp"
#include "glm/gtx/transform.hpp"
#include "../Mesh.h"

void NPC::initNPC()
{
    Points.emplace_back(glm::vec3(0,0.5,0));
    Points.emplace_back(glm::vec3(20,0.5,12));
    Points.emplace_back(glm::vec3(30,0.5,20));
    Points.emplace_back(glm::vec3(35,0.5,17));
}

glm::vec4 NPC::interpolation(std::vector<glm::vec3> points)
{
    glm::mat4 A{points[0].x * points[0].x * points[0].x, points[0].x * points[0].x, points[0].x, 1,
            points[1].x * points[1].x * points[1].x, points[1].x * points[1].x, points[1].x, 1,
            points[2].x * points[2].x * points[2].x, points[2].x * points[2].x, points[2].x, 1,
            points[3].x * points[3].x * points[3].x, points[3].x * points[3].x, points[3].x, 1};

    glm::vec4 z{points[0].z, points[1].z, points[2].z, points[3].z};

    glm::vec4 solution = z * glm::inverse(A);
    return solution;
}

float NPC::GetZ(glm::vec4 solution, float x)
{
    float a = solution.x;
    float b = solution.y;
    float c = solution.z;
    float d = solution.w;

    return a*x*x*x + b*x*x + c*x +d;
}

void NPC::updatePos(Cube& cube)
{
    if(cube.GetPosition().x >= 40.f)
        cubeMovement *= -1;

    if(cube.GetPosition().x <= 0.f)
        cubeMovement *= -1;

    cube.GetPosition().x += cubeMovement;
    cube.GetPosition().y = 0.5;
    cube.GetPosition().z = GetZ(interpolation(Points), cube.GetPosition().x);
}

