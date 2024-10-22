#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class UniformBuffer
{
public:
    void init();
    void uploadUniformObjData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    void cleanUp();
private:
    GLuint mUBO = 0;
};

#endif // UNIFORMBUFFER_H
