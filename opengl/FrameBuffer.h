#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FrameBuffer
{
public:
    bool init(unsigned int width, unsigned int height);
    bool resize(unsigned int newWidth, unsigned int newHeight);
    void bind();
    void unbind();
    void drawToScreen();
    void cleanup();
    GLuint getTexColorBuffer(){return mColorTex;}

private:
    unsigned int mBufferWidth = 640;
    unsigned int mBufferHeight = 480;
    GLuint mBuffer = 0;
    GLuint mColorTex = 0;
    GLuint mRendererBufferO = 0;
    bool checkComplete();
};

#endif // FRAMEBUFFER_H
