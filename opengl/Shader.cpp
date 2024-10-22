#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/type_trait.hpp>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
bool Shader::loadShaders(const std::string &vertexShaderFileName,const std::string &fragmentShaderFileName)
{
    Logger::log(1,"%s : is trying to open %s vertex shader file and %s fragment shader file.\n",__FUNCTION__,vertexShaderFileName.c_str(),fragmentShaderFileName.c_str());
    if(!createShaderProgram(vertexShaderFileName,fragmentShaderFileName)){
        Logger::log(1,"%s: shader program is not created.\n",__FUNCTION__);
        return false;
    }
    Logger::log(1,"%s: shaders is created.\n",__FUNCTION__);
    return true;
}
void Shader::use(){
    glUseProgram(mShaderProgram);
}
void Shader::cleanup(){
    glDeleteProgram(mShaderProgram);
}

GLuint Shader::getUniformLocation(const std::string& uniformName)
{
    return glGetUniformLocation(mShaderProgram,uniformName.c_str());
}

void Shader::setVec3(const glm::vec3& vector,const std::string& uniformName)
{
    glUniform3fv(getUniformLocation(uniformName), 1, &vector[0]);
}

void Shader::setMat4(const glm::mat4& matrix,const std::string& uniformName)
{
    glUniformMatrix4fv(getUniformLocation(uniformName),1,GL_FALSE,glm::value_ptr(matrix));
}

void Shader::setInt(const int &value, const std::string &uniformName)
{
    glUniform1i(getUniformLocation(uniformName),value);
}
void Shader::setFloat(const float& value, const std::string &uniformName){
    glUniform1f(getUniformLocation(uniformName),value);
}
bool Shader::createShaderProgram(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
{
    // Load Shaders
    GLuint vertexShader = loadShader(vertexShaderFileName,GL_VERTEX_SHADER);
    if(!vertexShader){
        Logger::log(1,"%s: %s failed to load Shader.\n",__FUNCTION__,vertexShaderFileName.c_str());
    }
    GLuint fragmentShader = loadShader(fragmentShaderFileName,GL_FRAGMENT_SHADER);
    if(!fragmentShader){
        Logger::log(1,"%s: %s failed to load Shader.\n",__FUNCTION__,fragmentShaderFileName.c_str());
    }
    // Crate a program
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram,vertexShader);
    glAttachShader(mShaderProgram,fragmentShader);
    glLinkProgram(mShaderProgram);
    if(!checkLinkStatus(mShaderProgram,vertexShaderFileName,fragmentShaderFileName)){
        Logger::log(1, "%s error: program linking from vertex shader '%s' / fragment shader '%s' failed\n", __FUNCTION__, vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return false;
    }
    // No longer need shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Logger::log(1, "%s: shader program %#x successfully compiled from vertex shader '%s' and fragment shader '%s'\n", __FUNCTION__, mShaderProgram, vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());
    return true;
}

GLuint Shader::loadShader(const std::string &shaderFileName, GLuint shaderType)
{
    std::string shaderAsText;
    // Read Shaders from file
    shaderAsText = loadFileToString(shaderFileName);
    // Create shader object
    const char* shaderSource = shaderAsText.c_str();
    GLuint shader = glCreateShader(shaderType);
    // Attach the shader source to the shader object
    glShaderSource(shader,1,&shaderSource,nullptr);
    glCompileShader(shader);

    if (!checkCompileStatus(shader, shaderFileName)) {
        Logger::log(1, "%s error: compiling shader '%s' failed\n", __FUNCTION__, shaderFileName.c_str());
        return 0;
    }

    Logger::log(1, "%s: shader %#x loaded and compiled\n", __FUNCTION__, shader);
    return shader;

}

std::string Shader::loadFileToString(const std::string& fileName)
{
    std::ifstream fileInput(fileName);
    std::string shaderStr;
    if(fileInput.is_open()){
        shaderStr.clear();
        // Get length of file
        fileInput.seekg(0,std::ios::end);
        // resize string
        shaderStr.reserve(fileInput.tellg());
        fileInput.seekg(0,std::ios::beg);
        shaderStr.assign(std::istreambuf_iterator<char>(fileInput),std::istreambuf_iterator<char>());
        fileInput.close();
    }else
    {
        Logger::log(1,"%s: %s cannot be opened.\n",__FUNCTION__,fileName.c_str());
        Logger::log(1,"The system says %s .\n",strerror(errno));
    }
    if (fileInput.bad() || fileInput.fail()) {
        Logger::log(1, "%s error: error while reading file %s\n", __FUNCTION__, fileName.c_str());
        fileInput.close();
        return std::string();
    }

    fileInput.close();
    Logger::log(1, "%s: file %s successfully read to string\n", __FUNCTION__, fileName.c_str());
    return shaderStr;
}

bool Shader::checkCompileStatus(GLuint shader, const std::string &shaderFileName)
{
    int success;
    int infoMessageLength;
    std::vector<char> infoLog;
    // Check compilation of shader
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success){
        // Get log message length if any
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoMessageLength);
        infoLog = std::vector<char>(infoMessageLength);
        glGetShaderInfoLog(shader,infoMessageLength,&infoMessageLength,infoLog.data());
        // Add nullterminated and of message
        infoLog.at(infoMessageLength) = '\0';
        Logger::log(1, "%s error: shader compile of shader '%s' failed\n", __FUNCTION__, shaderFileName.c_str());
        Logger::log(1, "%s compile log:\n%s\n", __FUNCTION__, infoLog.data());
        return false;
    }
    return true;
}

bool Shader::checkLinkStatus(GLuint shader, const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
{
    int success;
    int infoMessageLength;
    std::vector<char> infoLog;
    glGetProgramiv(shader,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramiv(shader,GL_INFO_LOG_LENGTH,&infoMessageLength);
        infoLog = std::vector<char>(infoMessageLength);
        glGetProgramInfoLog(shader,infoMessageLength,&infoMessageLength,infoLog.data());
        infoLog.at(infoMessageLength) = '\0';
        Logger::log(1, "%s error: program linking of shaders '%s' and '%s' failed\n", __FUNCTION__, vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());
        Logger::log(1, "%s compile log:\n%s\n", __FUNCTION__, infoLog.data());
        return false;
    }
    return true;
}
