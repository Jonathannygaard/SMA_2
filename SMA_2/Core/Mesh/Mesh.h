#pragma once
#include <memory>
#include "Collision/Collision.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Shader/Buffers/Buffers.h"

class Pickup;

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Color;
    glm::vec3 Normal = glm::vec3(0.f);
    Vertex(glm::vec3 Pos, glm::vec3 rgb) : Pos(Pos), Color(rgb)
    {
    }
    Vertex(glm::vec3 pos,glm::vec3 color,glm::vec3 normal)
        : Pos(pos),
          Color(color),
          Normal(normal)
    {
    }
};

struct Triangles
{
    unsigned int a, b, c;
    Triangles(unsigned int a, unsigned int b, unsigned int c) : a(a), b(b), c(c)
    {
    }
};

class Cube;
class Sphere;

class Mesh
{
    int count = 0;

public:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    std::vector<Triangles> tIndices;
    std::vector<std::shared_ptr<Cube>> Package;

	bool isTarrain = false;
    bool isLine = false;

    virtual void Draw(glm::mat4 model);
    virtual void Create(glm::vec3 color);

    virtual void BindBuffers();

    unsigned int VAO;
private:

    unsigned int VBO;
    unsigned int EBO;
};


class Cube : Mesh
{

public:
    Cube()
    {
    }
    bool bIsPlayer = false;
    bool bInteracted = false;
    bool bIsPickup = false;
    bool bShouldRender = true;
    bool bIsDoor = false;
    bool bDoorInteracted = false;

    float NPCDirection = 1;
    

    Cube* OverlappedCube = nullptr;

    void Create(glm::vec3 color) override;
    void Draw(glm::mat4 model) override;
    
    virtual ~Cube();

    void BindBuffers() override;

	unsigned int VAO;
private:
    
    unsigned int VBO;
    unsigned int EBO;

    std::vector<Vertex> cVertices;
    std::vector<Triangles> cIndices;
};

class Sphere : Mesh
{
    int index = 0;
    
    // How detailed the sphere is
    int Sectors = 16; 
    int Stacks = 16;

    //Radius of the sphere
    float radius = 1.f;

public:    
    Sphere()
    {
    }
    
    bool bOnGround = false;
    float Radius;
    float Mass;
    glm::vec3 Speed = glm::vec3(0.f);

    std::shared_ptr<Collision> Collider;
    
    void Create(glm::vec3 color) override;
    void Draw(glm::mat4 model) override;
    void BindBuffers() override;

    unsigned int VAO;
private:
    unsigned int VBO;
    unsigned int EBO;
    
    std::vector<Vertex> sVertices;
    std::vector<Triangles> sIndices; 
};


