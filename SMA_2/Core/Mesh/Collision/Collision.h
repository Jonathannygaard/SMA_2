#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>


#define PrintBool(x) std::cout << ((x) ? "True" : "False") << "\n"



enum class ECollisionType
{
    Wall,
    Interact,
    Door,
    Player,
    NPC,
    NoCollision,
    Collider,
    Sphere
};

class Sphere;
class Cube;

class Collision
{
public:
    glm::vec3 min;
    glm::vec3 max;
    float Radius;
    glm::vec3 scale;
    glm::vec3 offset;
    ECollisionType collisionType;

    Cube* cube;
    Sphere* sphere;

    Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset = glm::vec3(0.f),ECollisionType collision_type = ECollisionType::Collider, Cube* realCube = nullptr);
    Collision(glm::vec3 position, float radius, glm::vec3 offset = glm::vec3(0.f),ECollisionType collision_type = ECollisionType::Collider, Sphere* realSphere = nullptr);
    void UpdatePosition(glm::vec3 position);
    static void checkWorldCollision();
    static void CheckPickupCollisions();
    bool checkCollision(Collision& other);
    bool checkSphereCollision(Collision& other);
    void resolveCollision(Collision& other, glm::vec3 normal);
    void resolveSphereCollision(Collision& other);
    glm::vec3 lerp(glm::vec3& a, glm::vec3 b, float f);
    
    static std::vector<std::shared_ptr<Collision>> AllCollision;
    static std::vector<std::shared_ptr<Collision>> AllSphereCollision;
private:
    
    bool bIsCameraLock = false;
    bool HasOverlapped = false;
    float timer = 0.f;
    glm::vec3 OriginalCameraPosition = glm::vec3(0.f);
};
