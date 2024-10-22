#ifndef POSITIONGENERATOR_H
#define POSITIONGENERATOR_H

#include <glm/glm.hpp>
#include <vector>
#include <random>

class PositionGenerator
{
public:


    static float randomFloatGenerator(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-4, 4);
        return dis(gen);
    };

    static glm::vec3 randomCoordGenerator(){
        return glm::vec3(randomFloatGenerator(),randomFloatGenerator(),randomFloatGenerator());
    }


};

#endif // POSITIONGENERATOR_H
