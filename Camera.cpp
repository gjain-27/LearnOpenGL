#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 _cameraPos) {
	cameraPos = _cameraPos;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    movementSpeed = 2.5f;
    mouseSensitivity = 0.1f;

    yaw = -90.0f;
    pitch = 0.0f;

    zoom = 45.0f;
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

void Camera::ProcessMouseMovement(float xPosOffset, float yPosOffset) {
    yaw += xPosOffset * mouseSensitivity;
    pitch += yPosOffset * mouseSensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yPosOffset) {
    zoom -= static_cast<float>(yPosOffset);
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

float Camera::GetZoom() { return zoom; }

glm::vec3 Camera::getCameraPos() { return cameraPos; }
glm::vec3 Camera::getCameraFront() { return cameraFront; }

void Camera::updateCameraVectors() {
    glm::vec3 tempCameraFront;

    tempCameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    tempCameraFront.y = sin(glm::radians(pitch));
    tempCameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(tempCameraFront);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}