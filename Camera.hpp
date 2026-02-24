#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right
};

class Camera {
private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    float yaw;
    float pitch;

    void updateCameraVectors();

public:
    Camera(glm::vec3 _cameraPos);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xPosOffset, float yPosOffset);
    void ProcessMouseScroll(float yPosOffset);

    float GetZoom();
};