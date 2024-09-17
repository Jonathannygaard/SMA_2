#include "CollisionTree.h"


Node::Node(glm::vec3 Min, glm::vec3 Max, Node* Parent)
{
    min = Min;
    max = Max;

    parent = Parent;
    
}

void Node::AddCollision(Collision* collision)
{
    collisions.push_back(collision);
}

void Node::RemoveCollision(Collision* collision)
{    
    collisions.erase(std::find(collisions.begin(), collisions.end(), collision));
}

void Node::UpdateCollision()
{
  
}

void Node::devide()
    {
    glm::vec3 half = (max - min) / 2.f;
    glm::vec3 center = min + half;
    children[0] = new Node(min, center,this);
    children[1] = new Node(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z), this);
    children[2] = new Node(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z),this);
    children[3] = new Node(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z),this);
    children[4] = new Node(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z),this);
    children[5] = new Node(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z),this);
    children[6] = new Node(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z),this);
    children[7] = new Node(center, max,this);
}

void Node::removeChildren()
{
}

void Node::updateChildren()
{
    if(!children.empty())
    {
        for(Collision* c : collisions)
        {
            for(Node* n : children)
            {
                if(c->sphere != nullptr)
                {
                    
                }
            }
        }
    }
}
