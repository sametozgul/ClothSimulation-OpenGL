#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Logger.h"

class Shader
{
public:
    bool loadShaders(const std::string &vertexShaderFileName,const std::string &fragmentShaderFileName);
    GLuint getProgram(){return mShaderProgram;}
    void use();
    void cleanup();
    GLuint getUniformLocation(const std::string& uniformName);
    void setVec3(const glm::vec3& vector, const std::string& uniformName);
    void setMat4(const glm::mat4& matrix, const std::string& uniformName);
    void setInt(const int& value, const std::string& uniformName);
    void setFloat(const float &value, const std::string &uniformName);
private:
    bool createShaderProgram(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
    GLuint loadShader(const std::string &shaderFileName, GLuint shaderType);
    std::string loadFileToString(const std::string &fileName);
    bool checkCompileStatus(GLuint shader, const std::string &shaderFileName);
    bool checkLinkStatus(GLuint shader,const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
private:
    GLuint mShaderProgram = 0;
};

#endif // SHADER_H
