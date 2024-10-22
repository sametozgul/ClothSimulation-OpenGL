#include <glm/gtc/type_ptr.hpp>
#include "UniformBuffer.h"

void UniformBuffer::init(){
    glGenBuffers(1, &mUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);

    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::uploadUniformObjData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUBO, 0, 2 * sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::cleanUp()
{
    glDeleteBuffers(1, &mUBO);
}
