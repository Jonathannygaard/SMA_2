#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>


#define PrintBool(x) std::cout << ((x) ? "True" : "False") << "\n"

enum class ECollisionType
{
    Wall,
    Pickup,
    Door,
    Player,
    NPC,
    NoCollision,
    Collider
};

class Cube;

class Collision
{
public:
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 scale;
    glm::vec3 offset;
    ECollisionType collisionType;

    Cube* cube;

    Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset = glm::vec3(0.f),ECollisionType collision_type = ECollisionType::Collider, Cube* realCube = nullptr);
    void UpdatePosition(glm::vec3 position);
    static void checkWorldCollision();
    static void CheckPickupCollisions();
    bool checkCollision(Collision& other);
    glm::vec3 lerp(glm::vec3& a, glm::vec3 b, float f);

private:
    static std::vector<std::shared_ptr<Collision>> AllCollision;
    bool bIsCameraLock = false;
    bool HasOverlapped = false;
    float timer = 0.f;
    glm::vec3 OriginalCameraPosition = glm::vec3(0.f);
};
