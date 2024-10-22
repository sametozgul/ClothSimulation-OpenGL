#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle
{
public:
    Particle(const float& x, const float& y, bool pinned = false):
        position(x,y),
        previousPosition(x,y),
        acceleration(0.0f,0.0f),
        isPinned(pinned){};
    void applyForce(const glm::vec2& force);
    void constrainToBounds(const float& width, const float& height);
    void update(float time_step);
public:
    glm::vec2 position;
    glm::vec2 previousPosition;
    glm::vec2 acceleration;
    bool isPinned;
};

#endif // PARTICLE_H
