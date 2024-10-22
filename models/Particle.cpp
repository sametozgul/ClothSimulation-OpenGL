#include "Particle.h"


void Particle::applyForce(const glm::vec2 &force)
{
    if(!isPinned){
        acceleration += force;
    }
}

void Particle::constrainToBounds(const float &width, const float &height)
{
    if(position.x < 0) position.x = 0;
    if(position.x > width) position.x = width;
    if(position.y < 0) position.y = 0;
    if(position.y > height) position.y = height;
}
void Particle::update(float time_step) {
    // verlet intergration
    if (!isPinned) {
        glm::vec2 velocity = position - previousPosition;
        previousPosition = position;
        position += velocity + acceleration * time_step * time_step;
        acceleration = glm::vec2(0, 0); // reset after update
    }
}
