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

Collision collision(glm::vec3(9, 0.5, -6), glm::vec3(2, 2, 1.f));
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

	Sphere.CreateSphere(1.f,8,8, glm::vec3(0.f,10.f,0.f), glm::vec3(10.f), Color::Red);

	Terrain.CreateTerrain(-20,-20,20,20, 0.15);
	Terrain.isTarrain = true;
}

void Application::update() {
	
	//Terrain.FindTerrainHeight(Player.GetPosition());

	mCamera.OrbitCamera();
}

void Application::run() {
	create();
	float FirstFrame = 0.0f;

	glm::vec3 color(Color::Teal);
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
		Sphere.Draw();

		KeyBoardInput::processInput(mWindow, &Player);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();

}

Application::~Application()
{
}





