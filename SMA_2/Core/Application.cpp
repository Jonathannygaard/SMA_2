#define GLM_ENABLE_EXPERIMENTAL

#include "Application.h"
#include <iostream>
#include "glad/glad.h"
#include <glm/gtc/type_ptr.inl>
#include "Color.h"
#include "Camera/Camera.h"
#include "Window/Window.h"
#include "Input/Input.h"
#include "Mesh/Collision/Collision.h"
#include "Shader/Shader.h"
#include "Shader/Buffers/Buffers.h"
#include <GLFW/glfw3.h>

float timer = 0.f;
Camera Application::mCamera;
float Application::DeltaTime = 0.0f;


void Application::init() {
    mWindow = Window::init();
	mCamera.init();
	glLineWidth(5.f);
	
    run();
}


void Application::create() {

	// Player
	Player.CreateCube(glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.75f, 1.3f, 0.75f), Color::Purple);
	Player.bIsPlayer = true;

	sphere.CreateSphere(1.f, 1.f,16,16, glm::vec3(0.f,5.f,0.f), glm::vec3(1.f), Color::Green);
	sphere.AddCollider(1.f, ECollisionType::Sphere, glm::vec3(0.f));

	sphere2.CreateSphere(1.f,1.f,16,16, glm::vec3(10.f,1.f,0.f), glm::vec3(1.f), Color::Yellow);
	sphere2.AddCollider(1.f, ECollisionType::Sphere, glm::vec3(0.f));

	Floor.CreateCube(glm::vec3(-20.f,-0.5f,10.f), glm::vec3(40.f, 0.5f, 20.f), Color::Olive);
	Floor.AddCollider(glm::vec3(40.f, 0.5f, 20.f), ECollisionType::Wall, glm::vec3(0.f));

	Wall.CreateCube(glm::vec3(20.f,0.f,10.f), glm::vec3(1.f, 2.f, 20.f), Color::Red);
	Wall.AddCollider(glm::vec3(1.f,2.f,20.f), ECollisionType::Wall, glm::vec3(0.f));

	Wall2.CreateCube(glm::vec3(20.f,0.f,10.f), glm::vec3(-40.f, 2.f, 1.f), Color::Red);
	Wall2.AddCollider(glm::vec3(-40.f, 2.f, 1.f), ECollisionType::Wall, glm::vec3(0.f));

	Wall3.CreateCube(glm::vec3(-20.f,0.f,10.f), glm::vec3(1.f, 2.f, 20.f), Color::Red);
	Wall3.AddCollider(glm::vec3(1.f, 2.f, 20.f), ECollisionType::Wall, glm::vec3(0.f));

	Wall4.CreateCube(glm::vec3(-20.f,0.f,-10.f), glm::vec3(40.f, 2.f, 1.f), Color::Red);
	Wall4.AddCollider(glm::vec3(40.f, 2.f, 1.f), ECollisionType::Wall, glm::vec3(0.f));
	
}

void Application::update() {
	Floor.Collider->checkCollision(*sphere.Collider);
	for(auto s : Mesh::AllSpheres)
	{
		for (auto s2 : Mesh::AllSpheres)
		{
			if(s != s2)
			{
				s->Collider->checkSphereCollision(*s2->Collider);
			}
			s->Update(DeltaTime);
		}
	}
	mCamera.OrbitCamera();
}

void Application::run() {
	create();
	float FirstFrame = 0.0f;

	sphere.Speed = glm::vec3(2.f, 0.f, 0.f);

	glm::vec3 color(Color::Blue);
	while(!glfwWindowShouldClose(mWindow))
	{
		const auto CurrentFrame = static_cast<float>(glfwGetTime());
		DeltaTime = CurrentFrame - FirstFrame;
		FirstFrame = CurrentFrame;
		glClearColor(color.x, color.y, color.z, 1.f);
		glClear(GL_COLOR_BUFFER_BIT    | GL_DEPTH_BUFFER_BIT);
		glUseProgram(Shader::Program);
		
		update();
		
		glUniformMatrix4fv(mCamera.projectionLoc, 1, GL_FALSE, glm::value_ptr(mCamera.getProjection(Window::Width, Window::Height)));
		glUniformMatrix4fv(mCamera.viewLoc, 1, GL_FALSE, glm::value_ptr(mCamera.getView()));
		
		Player.Draw();
		Terrain.Draw();
		sphere.Draw();
		sphere2.Draw();
		Floor.Draw();
		Wall.Draw();
		Wall2.Draw();
		Wall3.Draw();
		Wall4.Draw();

		KeyBoardInput::processInput(mWindow, &Player);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();

}

Application::~Application()
{
}





