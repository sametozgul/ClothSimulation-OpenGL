/*----------------------------------------------------------------------
    FILE        : Camera.h
    AUTHOR      : Samet ÖZGÜL, Tubitak
    LAST UPDATE : 08.19.2024

    Header file of Camera can help Opengl View proccess.

    All Rights Free
-----------------------------------------------------------------------*/
#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    Camera();
    Camera(glm::vec3 position);

    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();

    void rotateAroundXAxis(float angle_in_degrees, float radius);

private:
    glm::mat4 mViewMatrix;
private:
    void updateView();
};

#endif // CAMERA_H
