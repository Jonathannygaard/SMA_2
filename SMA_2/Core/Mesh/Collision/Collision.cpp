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
    min = position + offset;
    max = position + scale;
    max.z = position.z - scale.z;
    collisionType = collision_type;
    cube = realCube;
    AllCollision.push_back(std::make_shared<Collision>(*this));
}

Collision::Collision(glm::vec3 position, float radius, glm::vec3 offset, ECollisionType collision_type, Sphere* realSphere)
{
    min = position + offset;
    Radius = radius;
    sphere = realSphere;
    collisionType = collision_type;
    AllSphereCollision.push_back(std::make_shared<Collision>(*this));
}

void Collision::UpdatePosition(glm::vec3 position)
{
    if(collisionType == ECollisionType::Sphere)
    {
        min = position + offset;
        return;
    }
    min = position + offset;
    max = position + scale;
    max.z = position.z - scale.z;
}


void Collision::checkWorldCollision()
{
    
}

bool Collision::checkCollision(Collision& other)
{
    if(collisionType == ECollisionType::Interact && other.collisionType == ECollisionType::Sphere)
    {
        glm::vec3 closestPoint = glm::clamp(other.min, min, max);
        closestPoint.z = glm::clamp (other.min.z, max.z, min.z);
        float distance = glm::distance(closestPoint, other.min);
        if(distance <= other.Radius)
        {
            if(cube->bInteracted)
            {
                glm::vec3 normal = glm::normalize(other.min - min);
                float Impulse = 5.f;
                other.sphere->Speed = normal * Impulse;
                return true;
            }
        }
    }
    if(other.collisionType == ECollisionType::Sphere && collisionType != ECollisionType::Interact)
    { 
        glm::vec3 closestPoint = glm::clamp(other.min, min, max);
        closestPoint.z = glm::clamp (other.min.z, max.z, min.z);
        float distance = glm::distance(closestPoint, other.min);
        if(distance <= other.Radius)
        {
            other.sphere->GetPosition() = closestPoint + (other.Radius * glm::normalize(other.min - closestPoint));
            resolveCollision(other, normalize(other.min - closestPoint));
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

void Collision::resolveCollision(Collision& other, glm::vec3 normal)
{
    glm::vec3 relativeVelocity = other.sphere->Speed;
    float VelosityAlongNormal = glm::dot(relativeVelocity, normal);

    if(VelosityAlongNormal > 0)
        return;

    float restitutuin = 1.f;
    float impulse = (-(1 + restitutuin) * VelosityAlongNormal) / (1 / other.sphere->Mass);

    float forceKept = 0.9f;
    glm::vec3 impulseVector = impulse * normal;
    other.sphere->Speed += (impulseVector / other.sphere->Mass) * forceKept;
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
