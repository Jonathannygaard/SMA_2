#pragma once
#include <vector>
#include "glad/glad.h"

struct Vertex;

class VertexArray
{
public:
    unsigned int va = 0;
    VertexArray() = default;
    ~VertexArray();

    void init();

    void Bind() const;
    void Unbind() const;
    void Activate() const;
};

class VertexBuffer
{
public:
    unsigned int vbo = 0;
    VertexBuffer() = default;
    void init();
    ~VertexBuffer();
    void Bind(const std::vector<Vertex>& objectVector) const;
};

class ElementBuffer
{
public:
    unsigned int ebo = 0;
    ElementBuffer() = default;
    ~ElementBuffer();
    void init();
    template<typename T>
    void Bind(const std::vector<T>& objectVector) const;
};

template<typename T>
void ElementBuffer::Bind(const std::vector<T>& objectVector) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objectVector.size() * sizeof(T), objectVector.data(), GL_STATIC_DRAW);
}



