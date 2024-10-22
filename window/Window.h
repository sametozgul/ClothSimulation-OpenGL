#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Logger.h"
#include "OGLRenderer.h"
class Window
{
public:
    bool init(unsigned int width, unsigned int height, std::string title);
    void mainLoop();
    void cleanUp();
private:
    GLFWwindow* mWindow = nullptr;

    std::unique_ptr<OGLRenderer> mRenderer;
};

#endif // WINDOW_H
