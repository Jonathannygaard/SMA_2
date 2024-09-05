#include "Input.h"
#include <iostream>
#include "../Application.h"
#include "GLFW/glfw3.h"

void Input::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void KeyBoardInput::processInput(GLFWwindow* window, Cube* player)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
	{
		player->GetPosition().x += Application::mCamera.cameraSpeed * Application::mCamera.cameraFront.x * Application::DeltaTime;
		player->GetPosition().z += Application::mCamera.cameraSpeed * Application::mCamera.cameraFront.z * Application::DeltaTime;
		Application::mCamera.setPlayerPos(player->GetPosition());
	}
	if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
	{
		player->GetPosition().x -= Application::mCamera.cameraSpeed * Application::mCamera.cameraFront.x * Application::DeltaTime;
		player->GetPosition().z -= Application::mCamera.cameraSpeed * Application::mCamera.cameraFront.z * Application::DeltaTime;
		Application::mCamera.setPlayerPos(player->GetPosition());
	}
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
	{
		player->GetPosition() -= (Application::mCamera.cameraSpeed * glm::normalize(glm::cross(Application::mCamera.cameraFront, Application::mCamera.cameraUp))) * Application::DeltaTime;
		Application::mCamera.setPlayerPos(player->GetPosition());
	}
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
	{
		player->GetPosition() += (Application::mCamera.cameraSpeed * glm::normalize(glm::cross(Application::mCamera.cameraFront, Application::mCamera.cameraUp))) * Application::DeltaTime;
		Application::mCamera.setPlayerPos(player->GetPosition());
	}
	if(glfwGetKey(window, GLFW_KEY_E)==GLFW_PRESS)
	{
		player->GetPosition().y += Application::mCamera.cameraSpeed * Application::DeltaTime;
		Application::mCamera.setPlayerPos(player->GetPosition());
	}
	if(glfwGetKey(window, GLFW_KEY_Q)==GLFW_PRESS)
	{
		player->GetPosition().y -= Application::mCamera.cameraSpeed * Application::DeltaTime;
		Application::mCamera.setPlayerPos(player->GetPosition());
	}
	
}

namespace MouseInput
{
	bool firstMouse = true;
	double lastX = 0.f;
	double lastY = 0.f;
	float yaw = -90.f;
	float pitch = 0.f;
	float fov = 45.f;
}


void MouseInput::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!Application::mCamera.CameraLock)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		auto xoffset = static_cast<float>(xpos - lastX);
		auto yoffset = static_cast<float>(lastY - ypos);
		lastX = xpos;
		lastY = ypos;
		const float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		Application::mCamera.cameraFront = glm::normalize(direction);
	}

}
