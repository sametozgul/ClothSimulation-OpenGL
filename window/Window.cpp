#include "Window.h"

bool Window::init(unsigned int width, unsigned int height, std::string title)
{
    if(!glfwInit()){
        Logger::log(1,"%s : GLFW is not initialized.\n",__FUNCTION__);
        return false;
    }
    Logger::log(1,"%s : GLFW is initialized.\n",__FUNCTION__);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(width,height,title.c_str(),nullptr,nullptr);

    if(mWindow == nullptr){
        Logger::log(1,"%s : Window is not created.\n",__FUNCTION__);
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mWindow);

    if(!mRenderer){
        mRenderer = std::make_unique<OGLRenderer>(mWindow);
    }
    glfwSetWindowUserPointer(mWindow, mRenderer.get());
    glfwSetWindowSizeCallback(mWindow,[](GLFWwindow* win,int width,int height){
        auto renderer = static_cast<OGLRenderer*>(glfwGetWindowUserPointer(win));
        renderer->resize(width,height);
    });
    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *win, int button, int action, int mods){
        auto renderer = static_cast<OGLRenderer*>(glfwGetWindowUserPointer(win));
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(win, &xpos, &ypos);
            renderer->handleMousePressEvents(xpos, ypos);
        }else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
            renderer->handleMouseReleasedEvents();
        }
    });
    if(!mRenderer->init(width,height)){
        glfwTerminate();
        Logger::log(1,"%s : Renderer is not initialized...\n",__FUNCTION__);
    }
    return true;
}
void Window::mainLoop()
{
    glfwSwapInterval(1);
    while(!glfwWindowShouldClose(mWindow)){
        mRenderer->draw();
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        double xpos, ypos;
        glfwGetCursorPos(mWindow, &xpos, &ypos);
        mRenderer->checkMouseStates(xpos, ypos);
    }
}
void Window::cleanUp()
{
    mRenderer->cleanUp();
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    Logger::log(1,"%s : Window is terminated.\n",__FUNCTION__);
}
