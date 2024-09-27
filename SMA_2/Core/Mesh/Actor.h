#pragma once
#include "Mesh.h"

class Actor
{
public:
    Mesh* mesh;

    int MeshIndex;
    
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
    float Mass;

    glm::vec3 Speed;

    std::vector<std::unique_ptr<Collision>> Colliders;

    Actor(Mesh* Mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    glm::vec3 GetPosition(){return Position;}
    glm::vec3 GetRotation(){return Rotation;}
    glm::vec3 GetScale(){return Scale;}
    
    void Update();
    void Move(glm::vec3 position);
    glm::mat4 GetModelMatrix();

    void AddCollider(glm::vec3 scale,ECollisionType collisionType, glm::vec3 offset = glm::vec3(0.f));
};
