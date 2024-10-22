#include "Constraint.h"


Constraint::Constraint(Particle *p1, Particle *p2)
{
    p1_ = p1;
    p2_ = p2;
    initialLength =   std::hypot(p2_->position.x - p1_->position.x,
                                                p2_->position.y - p1_->position.y);
    active = true;

}

void Constraint::satisfy()
{
    if(!active) return;
    glm::vec2 delta = p2_->position - p1_->position;
    float currentLength = std::hypot(delta.x, delta.y);
    float difference = (currentLength - initialLength) / currentLength;
    glm::vec2 correction = delta * 0.5f * difference;

    if (!p1_->isPinned) p1_->position += correction;
    if (!p2_->isPinned) p2_->position -= correction;
}
void Constraint::deactivate() {
    active = false;
}
