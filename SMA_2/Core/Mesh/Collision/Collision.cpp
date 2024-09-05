#define GLM_ENABLE_EXPERIMENTAL

#include "Collision.h"
#include <iostream>
#include "../../Application.h"
#include "../Mesh.h"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtx/dual_quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

std::vector<std::shared_ptr<Collision>> Collision::AllCollision;

Collision::Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset, ECollisionType collision_type, Cube* realCube) : scale(scale), offset(offset), collisionType(collision_type)
{
    min = position;
    max = position + scale;
    max.z = position.z - scale.z;

    cube = realCube;
    AllCollision.push_back(std::make_shared<Collision>(*this));
}

Collision::Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset, ECollisionType collision_type,
    Sphere* realSphere)
{
    min = position;
    max = position + scale;
    sphere = realSphere;
    AllCollision.push_back(std::make_shared<Collision>(*this));
}

void Collision::UpdatePosition(glm::vec3 position)
{
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
    if(other.min.x < max.x && other.max.x > min.x &&
        other.min.y < max.y && other.max.y > min.y &&
        max.z <= other.min.z && min.z >= other.max.z)
    {
        if(cube == nullptr && other.cube == nullptr)
            return false;

        if(cube != nullptr)
        {
             if (cube->bIsPlayer)
             {
                 cube->bCanInteract = true;
                 if(other.cube)
                    cube->OverlappedCube = other.cube;
             }
            if(collisionType == ECollisionType::Wall)
            {

            }
        }
        else
        {
            if (HasOverlapped == false)
            {
                if(bIsCameraLock == false)
                {
                    HasOverlapped = true;
                    timer = 0.f;
                    Application::mCamera.cameraFront = glm::vec3(-1.f, -0.2f, -1.f);
                    Application::mCamera.CameraLock = true;
                    bIsCameraLock = true;
                    OriginalCameraPosition = Application::mCamera.cameraPos;
                }
                else
                {
                    HasOverlapped = true;
                    timer = 0.f;
                    Application::mCamera.cameraFront = glm::vec3(0.f,0.f, -1.f);
                    Application::mCamera.CameraLock = false;
                    bIsCameraLock = false;
                }
            }
            else
            {
                if(bIsCameraLock == true)
                {
                    timer += Application::DeltaTime * 0.5f;
                    timer = glm::clamp(timer, 0.f, 1.01f);
                    lerp(Application::mCamera.cameraPos, glm::vec3(14.8, 2.5, -5.2), timer);
                }
                else
                {
                }
            }
        }
       if (other.cube != nullptr)
       {
           if (other.cube->bIsPlayer)
           {
               other.cube->bCanInteract = true;
               if(cube)
                other.cube->OverlappedCube = cube;
           }
       }
         return true;
    }
    if (HasOverlapped == true && bIsCameraLock == false)
        Application::mCamera.OrbitCamera();
    HasOverlapped = false;

    return false;
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
