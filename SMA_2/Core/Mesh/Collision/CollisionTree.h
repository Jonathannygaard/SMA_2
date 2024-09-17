#pragma once
#include <array>
#include "Collision.h"

struct Node
{
    Node* parent = nullptr;
    glm::vec3 min;
    glm::vec3 max;

    std::array<Node*, 8> children;
    std::vector<Collision*> collisions;

    Node(glm::vec3 Min, glm::vec3 Max, Node* Parent = nullptr);

    void AddCollision(Collision* collision);
    void RemoveCollision(Collision* collision);
    void UpdateCollision();
    
    void devide();
    void removeChildren();
    void updateChildren();
};

class CollisionTree
{
public:
    glm::vec3 center;
    glm::vec3 min;
    glm::vec3 max;

    Node* root = nullptr;
};
