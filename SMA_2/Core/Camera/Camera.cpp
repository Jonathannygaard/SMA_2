//
// Created by jonat on 08/04/2024.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include <glad/glad.h>
#include "../Shader/Shader.h"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"

void Camera::init() {
    viewLoc = glGetUniformLocation(Shader::Program, "view");
    projectionLoc = glGetUniformLocation(Shader::Program, "projection");
}

glm::mat4 Camera::getView() {
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    return view;
}
 
glm::mat4 Camera::getProjection(float width, float height) {
    glm::mat4 proj = glm::mat4(1.f);
    proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
    return proj;
}

void Camera::setPlayerPos(glm::vec3 position) {
    PlayerPos = glm::vec3(position.x + 0.375f, position.y + 0.65f, position.z - 0.375f);
}

glm::vec3 Camera::OrbitCamera()
{
    cameraPos = glm::vec3(PlayerPos.x,PlayerPos.y,PlayerPos.z) + cameraFront * -5.f;
    return cameraPos;
}

void Camera::updatePos(glm::vec3 newpos) {
    cameraPos += newpos;
    cameraFront += newpos;
}
