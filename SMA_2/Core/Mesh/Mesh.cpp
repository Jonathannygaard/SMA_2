#define GLM_ENABLE_EXPERIMENTAL

#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>
#include "../Shader/Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "../Color.h"



void Cube::CreateCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color, bool isPickup, bool isPlayer, glm::vec3 rotation, bool isDoor)
{
	Position = position;
	Scale = scale;
	bIsPlayer = isPlayer;
	bIsPickup = isPickup;
	bIsDoor = isDoor;

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

void Cube::Draw()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, GetPosition());
	//model = glm::scale(model, cube->GetScale());
	glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, cIndices.size()*3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}

void Cube::AddCollider(glm::vec3 scale, ECollisionType collisionType, glm::vec3 offset)
{
	Collider = std::make_unique<Collision>(GetPosition()+offset,scale, offset,collisionType,this);
}

void Cube::MoveNPC(Cube& NPC, glm::vec3 pos)
{
	NPC.Position = pos;
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

void Sphere::CreateSphere(float Radius,int Sectors,int Stacks, glm::vec3 position, glm::vec3 color)
{
	float SectorStep = 2 * glm::pi<float>() / Sectors;
	float StackStep = glm::pi<float>() / Stacks;
	float SectorAngle, StackAngle;

	for(int i = 0; i >= Stacks; i++)
	{
		StackAngle = glm::pi<float>() / 2 - i * StackStep;
		float xy = Radius * cos(StackAngle);
		float z = Radius * sin(StackAngle);

		for(int j = 0; j >= Sectors; j++)
		{
			SectorAngle = j * SectorStep;

			float x = xy * cos(SectorAngle);
			float y = xy * sin(SectorAngle);

			glm::vec3 Temp(x, y, z);
			Temp += position;
			Position = position;
			sVertices.emplace_back(Temp, color);
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
}

void Sphere::Draw()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, GetPosition());
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


void Mesh::BindBuffers()
{
	// VBO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// EBO
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tIndices.size() * sizeof(Triangles), tIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Pos)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

float Mesh::f(float x)
{
	return x * x * x - 2 * x;
}

void Mesh::CreateCurve(Mesh ThePlane)
{
	for (float i = -20; i < 20; i += 0.1)
	{
		float x = i;
		float y = 0;
		float z = f(i);


		glm::vec3 Temp(x, y, z);
		if (FindTerrainHeight(Temp)) 
		{
			mVertices.emplace_back(Temp, Color::Red);
		}

	}
	BindBuffers();
}


float Mesh::xy(float t, float u)
{
	return cos(t) + sin(u);
}

void Mesh::CreateTerrain(float xStart, float yStart, float xEnd, float yEnd, float Step)
{
	float xLength = 0.f;
	float zLength = 0.f;

	for (float i = xStart; i < xEnd; i += Step)
	{
		xLength++;
		for (float j = yStart; j < yEnd; j += Step)
		{
			if(i == xStart)
			{
				zLength++;
			}
			mVertices.emplace_back(glm::vec3(i, 1.f, j), Color::Coral);
		}
	}
	float index = 1;
	
	for(int x = 0; x < xLength - 1; x++)
	{
			for (int z = 0; z < zLength - 1; z++)
			{
				tIndices.emplace_back(x * zLength + z, index * zLength + z, index * zLength + z + 1);
				tIndices.emplace_back(index * zLength + z + 1, x * zLength + z + 1, x * zLength + z);
			}
			index++;
	}
	for (auto Triangles: tIndices)
	{
		glm::vec3 Normal = glm::normalize(glm::cross(mVertices[Triangles.c].Pos - mVertices[Triangles.a].Pos,
			mVertices[Triangles.b].Pos - mVertices[Triangles.a].Pos));
		mVertices[Triangles.a].Normal += Normal;
		mVertices[Triangles.b].Normal += Normal;
		mVertices[Triangles.c].Normal += Normal;
	}
	BindBuffers();
}

void Mesh::Draw()
{
	
	glm::mat4 model(1.f);
	model = glm::translate(model, glm::vec3(0.f));
	glBindVertexArray(VAO);
	glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	
	if(!isLine)
	{
		glDrawElements(GL_TRIANGLES, tIndices.size()*3, GL_UNSIGNED_INT, nullptr);
	}
	if (isLine)
	{
		glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
	}
	glBindVertexArray(0);
	
}

float Mesh::calculate_Normal(const glm::vec3&& AB, const glm::vec3&& AC)
{
	return AB[0] * AC[2] - AC[0] * AB[2];
}

bool Mesh::FindTerrainHeight(glm::vec3& Player)
{
	glm::vec3 p1, p2, p3,player, temp;
	for(auto Triangle: tIndices)
	{
		p1 = mVertices[Triangle.a].Pos;
		p2 = mVertices[Triangle.b].Pos;
		p3 = mVertices[Triangle.c].Pos;
		player = Player;

		float tArea = glm::length(calculate_Normal(p2-p1, p3-p1));

		float u = (calculate_Normal(p2 - player, p3 - player)) / tArea;
		float v = (calculate_Normal(p3 - player, p1 - player)) / tArea;
		float w = (calculate_Normal(p1 - player, p2 - player)) / tArea;

		if (w >= 0 && v >= 0 && u >= 0)
		{
			float xCoord = u * p1.x + v * p2.x + w * p3.x;
			float yCoord = u * p1.y + v * p2.y + w * p3.y;
			float zCoord = u * p1.z + v * p2.z + w * p3.z;
			Player.y = yCoord;
			return true;
		}
	}
	return false;
}
