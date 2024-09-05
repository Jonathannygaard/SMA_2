#include "Buffers.h"
#include "../../Mesh/Mesh.h"
#include "glad/glad.h"

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &va);
}

void VertexArray::init()
{
	glGenVertexArrays(1, &va);
    Bind();
}

void VertexArray::Bind() const
{
    glBindVertexArray(va);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::Activate() const
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 1);
    glBindVertexArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 2);
    glBindVertexArray(2);
}

void VertexBuffer::init()
{
	glGenBuffers(1, &vbo);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &vbo);
}

void VertexBuffer::Bind(const std::vector<Vertex>& objectVector) const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, objectVector.size() * sizeof(Vertex), objectVector.data(), GL_STATIC_DRAW);

}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &ebo);
}

void ElementBuffer::init()
{
    glGenBuffers(1, &ebo);
}

