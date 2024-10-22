#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Particle.h"
#include <cmath>
#include <limits>
class Constraint
{
public:
    Constraint(Particle *p1, Particle *p2);
    void satisfy();
    void deactivate();
public:
    Particle *p1_;
    Particle *p2_;
    float initialLength;
    bool active;
};

#endif // CONSTRAINT_H
