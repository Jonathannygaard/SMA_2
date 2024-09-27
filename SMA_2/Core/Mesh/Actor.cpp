#include "Actor.h"
#include "../Shader/Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "../Application.h"

Actor::Actor(Mesh* Mesh, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    mesh = Mesh;
    Position = position;
    Rotation = rotation;
}

void Actor::Update()
{
    Speed.y += -9.81f * Application::DeltaTime;
    Position += Speed * Application::DeltaTime;
    
    for (auto c : Colliders)
    {
        c->UpdatePosition(Position);
    }
    
}

void Actor::Move(glm::vec3 position)
{
}

glm::mat4 Actor::GetModelMatrix()
{
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, Position);
    model = glm::scale(model, Scale);
}

void Actor::AddCollider(glm::vec3 scale, ECollisionType collisionType, glm::vec3 offset)
{
    Colliders.push_back(std::make_unique<Collision>(GetPosition(),scale, offset,collisionType,mesh));
}
