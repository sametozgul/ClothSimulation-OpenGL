#include "Camera.h"
#include <iostream>

Camera::Camera()
{
    Position = glm::vec3(0.0f,0.0f,3.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3(0.0f, 0.0f, 1.0f);
    updateView();
}

Camera::Camera(glm::vec3 position)
{
    Position = position;
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Front = glm::vec3(0.0f, 0.0f, 1.0f);
    Right = glm::normalize(glm::cross(Front, Up));  // Initialize Right vector
    updateView();  // Initialize the view matrix
}

glm::mat4 Camera::getViewMatrix()
{
    return mViewMatrix;
}

glm::vec3 Camera::getPosition()
{
    return Position;
}

void Camera::rotateAroundXAxis(float angle_in_degrees, float radius)
{
    float angle_in_radians = glm::radians(angle_in_degrees);
    Position.x = radius * cos(angle_in_radians);
    Position.z = radius * sin(angle_in_radians);
    Position.y = 0.0f;
    Front = glm::normalize(-Position);  // The cube is at the origin (0, 0, 0)
    mViewMatrix = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
}

void Camera::updateView()
{
    // Create the view matrix based on the updated camera vectors
    mViewMatrix = glm::lookAt(Position, Position + Front, Up);
}
