#include "FrameBuffer.h"
#include "Logger.h"

bool FrameBuffer::init(unsigned int width, unsigned int height)
{
    mBufferWidth = width;
    mBufferHeight = height;

    glGenFramebuffers(1,&mBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);

    glGenTextures(1, &mColorTex);
    glBindTexture(GL_TEXTURE_2D, mColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTex, 0);

    glGenRenderbuffers(1, &mRendererBufferO);
    glBindRenderbuffer(GL_RENDERBUFFER, mRendererBufferO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRendererBufferO);

    return checkComplete();
}

bool FrameBuffer::resize(unsigned int newWidth, unsigned int newHeight)
{
    mBufferWidth = newWidth;
    mBufferHeight = newHeight;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &mColorTex);
    glDeleteRenderbuffers(1, &mRendererBufferO);
    glDeleteFramebuffers(1, &mBuffer);
    return init(newWidth, newHeight);
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::drawToScreen()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, mBufferWidth, mBufferHeight, 0, 0, mBufferWidth, mBufferHeight,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::cleanup()
{
    glDeleteFramebuffers(1,&mBuffer);
    glDeleteRenderbuffers(1, &mRendererBufferO);
}

bool FrameBuffer::checkComplete()
{
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE) {
        Logger::log(1, "%s error: framebuffer is NOT complete\n", __FUNCTION__);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Logger::log(1, "%s: framebuffer is complete\n", __FUNCTION__);
    return true;
}
