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

    float movementSpeed;

public:
    Camera(glm::vec3 _cameraPos);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
};