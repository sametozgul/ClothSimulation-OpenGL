#pragma once
#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "OGLRenderData.h"
#include "Logger.h"
class UserInterface
{
public:
    void init(OGLRenderData &renderData);
    void createFrame(OGLRenderData &renderData);
    void render();
    void cleanUp();
private:
    float framesPerSecond = 0.0f;
    float averagingAlpha = 0.96f;
};

#endif // USERINTERFACE_H
