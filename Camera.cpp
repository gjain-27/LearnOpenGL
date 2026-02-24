#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 _cameraPos) {
	cameraPos = _cameraPos;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    movementSpeed = 2.5f;
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    glm::vec3 moveDir(0.0f);

    if (direction == CameraMovement::Forward)
        moveDir += cameraFront;
    if (direction == CameraMovement::Backward)
        moveDir -= cameraFront;
    if (direction == CameraMovement::Left)
        moveDir -= cameraRight;
    if (direction == CameraMovement::Right)
        moveDir += cameraRight;

    if (glm::length(moveDir) > 0.0f)
        moveDir = glm::normalize(moveDir);

    cameraPos += moveDir * movementSpeed * deltaTime;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}