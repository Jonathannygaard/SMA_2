#define GLM_ENABLE_EXPERIMENTAL

#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>
#include <windows.h>
#include "../Application.h"
#include "../Shader/Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "../Color.h"

void Cube::Create(glm::vec3 color)
{
	Vertex v0{glm::vec3(0.f, 0.f, 0.f), color}; /* Front-Bot-left */
	Vertex v1{ glm::vec3(1.f, 0.f, 0.f), color}; /* Front-Bot-right */
	Vertex v2{ glm::vec3(1.f, 1.f, 0.f), color}; /* Front-Top-right */
	Vertex v3{ glm::vec3(0.f, 1.f, 0.f), color}; /* Front-Top-left */
	Vertex v4{ glm::vec3(0.f, 0.f, -1.f), color}; /* Back-Bot-left */
	Vertex v5{ glm::vec3(1.f, 0.f, -1.f), color}; /* Back-Bot-right */
	Vertex v6{ glm::vec3(1.f, 1.f, -1.f), color}; /* Back-Top-right */
	Vertex v7{ glm::vec3(0.f, 1.f, -1.f), color}; /* Back-Top-left */

	cVertices.emplace_back(v0);
	cVertices.emplace_back(v1);
	cVertices.emplace_back(v2);
	cVertices.emplace_back(v3);
	cVertices.emplace_back(v4);
	cVertices.emplace_back(v5);
	cVertices.emplace_back(v6);
	cVertices.emplace_back(v7);

	/* Front */
	cIndices.emplace_back(0,1,2);
	cIndices.emplace_back(2,3,0);

	/* Right */
	cIndices.emplace_back(1,5,6);
	cIndices.emplace_back(6,2,1);

	/* Left */			   
	cIndices.emplace_back(0,3,7);
	cIndices.emplace_back(7,4,0);
						   
	/* Back */
	cIndices.emplace_back(4,7,6);
	cIndices.emplace_back(6,5,4);

	/* Top */
	cIndices.emplace_back(3,2,6);
	cIndices.emplace_back(6,7,3);

	/* Bottom */
	cIndices.emplace_back(0,4,5);
	cIndices.emplace_back(5,1,0);

	for (auto Triangles : cIndices)
	{
		glm::vec3 Normal = glm::normalize(glm::cross(cVertices[Triangles.b].Pos - cVertices[Triangles.a].Pos,
			cVertices[Triangles.c].Pos - cVertices[Triangles.a].Pos));
		cVertices[Triangles.a].Normal += Normal;
		cVertices[Triangles.b].Normal += Normal;
		cVertices[Triangles.c].Normal += Normal;
	}
	BindBuffers();
}

void Cube::Draw(glm::mat4 model)
{
	glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, cIndices.size()*3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}

Cube::~Cube()
{
}

void Cube::BindBuffers()
{
	// VAO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, cVertices.size() * sizeof(Vertex), cVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cIndices.size() * sizeof(Triangles), cIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sphere::Create(glm::vec3 color)
{
	float x, y, z, xy;
	float SectorStep = 2 * glm::pi<float>() / Sectors;
	float StackStep = glm::pi<float>() / Stacks;
	float SectorAngle, StackAngle;

	for(int i = 0; i <= Stacks; i++)
	{
		StackAngle = glm::pi<float>() / 2 - i * StackStep;
		float xy = radius * cos(StackAngle);
		float z = radius * sin(StackAngle);
		
		for(int j = 0; j <= Sectors; j++)
		{
			SectorAngle = j * SectorStep;

			float x = xy * cos(SectorAngle);
			float y = xy * sin(SectorAngle);
			glm::vec3 Temp(x,y,z);
			if(i == 0 && j != 0)
			{
				//continue;
			}
				
			sVertices.emplace_back(Temp, color, Temp);
		}
	}	

	for(int i = 0; i < Stacks; i++)
	{
		int k1 = i * (Sectors + 1);
		int k2 = k1 + Sectors + 1;
	
		for(int j = 0; j < Sectors; j++, k1++, k2++)
		{
			if(i != 0)
			{
				sIndices.emplace_back(k1, k2, k1 + 1);
			}
			if(i != (Stacks - 1))
			{
				sIndices.emplace_back(k1 + 1, k2, k2 + 1);
			}
		}
	}
	BindBuffers();	
}

void Sphere::Draw(glm::mat4 model)
{
	glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sIndices.size()*3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Sphere::BindBuffers()
{
	// VAO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sVertices.size() * sizeof(Vertex), sVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sIndices.size() * sizeof(Triangles), sIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}