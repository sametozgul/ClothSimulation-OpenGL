#pragma once
#ifndef OGLRENDERDATA_H
#define OGLRENDERDATA_H

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
struct OGLVertex {
    glm::vec2 position;
    glm::vec3 color;
};

struct OGLMesh {
    std::vector<OGLVertex> vertices;
};

struct OGLRenderData{
    GLFWwindow *rdWindow = nullptr;

    int rdWidth = 0;
    int rdHeight = 0;
    int rdNrAttributes = 0;
    int rdTriangleCount = 0 ;

    bool rdUsePolygonMode = false;

    bool rdIsColorChanged = false;
    int rdVertexColorPosition = 0;
    int rdVertexObjectColorPosition = 0;
    int rdVertexLightColorPosition = 0;

    float rdXLastPos = 0;
    float rdYLastPos = 0;

    float rdYaw = 0.0f;
    float rdPitch = 0.0f;
    float rdFov = 45.0f;

    int gravity = 10;

    float rdTickDiff = 0.0f;
    float rdFrameTime = 0.0f;
};

#endif // OGLRENDERDATA_H
