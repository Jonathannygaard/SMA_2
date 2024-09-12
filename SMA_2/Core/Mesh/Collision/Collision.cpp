#define GLM_ENABLE_EXPERIMENTAL

#include "Collision.h"
#include <iostream>
#include "../../Application.h"
#include "../Mesh.h"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtx/dual_quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

std::vector<std::shared_ptr<Collision>> Collision::AllCollision;
std::vector<std::shared_ptr<Collision>> Collision::AllSphereCollision;

Collision::Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset, ECollisionType collision_type, Cube* realCube) : scale(scale), offset(offset), collisionType(collision_type)
{
    min = position;
    max = position + scale;
    max.z = position.z - scale.z;
    collisionType = collision_type;
    cube = realCube;
    AllCollision.push_back(std::make_shared<Collision>(*this));
}

Collision::Collision(glm::vec3 position, float radius, glm::vec3 offset, ECollisionType collision_type,
    Sphere* realSphere)
{
    min = position;
    Radius = radius;
    sphere = realSphere;
    collisionType = collision_type;
    AllSphereCollision.push_back(std::make_shared<Collision>(*this));
}

void Collision::UpdatePosition(glm::vec3 position)
{
    if(collisionType == ECollisionType::Sphere)
    {
        min = position;
        return;
    }
    min = position;
    max = position + scale;
    max.z = position.z - scale.z;
}


void Collision::checkWorldCollision()
{
    
}

void Collision::CheckPickupCollisions()
{
    for(auto& player : AllCollision)
    {
        if(player->cube)
        {
            if (player->cube->bIsPlayer)
            {
                for (auto& element : AllCollision)
                {
                    if (element->collisionType == ECollisionType::Pickup)
                    {
                        player->checkCollision(*element);
                    }
                }
                break;
            }
        }

    }
}

bool Collision::checkCollision(Collision& other)
{
    if(other.collisionType == ECollisionType::Sphere)
    {
        if(cube == nullptr && other.sphere == nullptr)
        {
            return false;
        }
                     
        if(min.x < other.min.x - other.Radius &&
            max.x > other.min.x + other.Radius &&
            min.y < other.min.y + other.Radius &&
            max.y > other.min.y - other.Radius &&
            min.z > other.min.z + other.Radius &&
            max.z < other.min.z - other.Radius)
        {
            resolveCollision(other);
            return true;
        }
    }
    return false;
}

bool Collision::checkSphereCollision(Collision& other)
{
    if(collisionType == ECollisionType::Sphere && other.collisionType == ECollisionType::Sphere)
    {
        if(glm::distance(min, other.min) < Radius + other.Radius)
        {
            resolveSphereCollision(other);
            return true;
        }
    }
    return false;
}

void Collision::resolveCollision(Collision& other)
{
    glm::vec3 floorNormal = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 relativeVelocity = other.sphere->Speed;
    float VelosityAlongNormal = glm::dot(relativeVelocity, floorNormal);

    if(VelosityAlongNormal > 0)
        return;

    float restitutuin = 1.f;
    float impulse = (-(1 + restitutuin) * VelosityAlongNormal) / (1 / other.sphere->Mass);

    glm::vec3 impulseVector = impulse * floorNormal;
    other.sphere->Speed += impulseVector / other.sphere->Mass;
}

void Collision::resolveSphereCollision(Collision& other)
{
    glm::vec3 normal = glm::normalize(min - other.min);
    glm::vec3 relativeVelocity = sphere->Speed - other.sphere->Speed;
    float VelosityAlongNormal = glm::dot(relativeVelocity, normal);

    if(VelosityAlongNormal > 0)
        return;

    float restitution = 1.f; //perfect elasticity
    float impulse = (-(1 + restitution) * VelosityAlongNormal) / (1 / sphere->Mass + 1 / other.sphere->Mass);

    glm::vec3 impulseVector = impulse * normal;
    sphere->Speed += impulseVector / sphere->Mass;
    other.sphere->Speed -= impulseVector / other.sphere->Mass;
}

glm::vec3 Collision::lerp(glm::vec3& a, glm::vec3 b, float f)
{
    if (f >= 1.f)
    {
        return Application::mCamera.cameraPos;
    }

    a = glm::mix(Application::mCamera.cameraPos, glm::vec3(14.8f, 2.5f, -5.2f), f);
    return Application::mCamera.cameraPos;
}
