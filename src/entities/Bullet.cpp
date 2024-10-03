#include "entities/Bullet.h"
#include <cmath>

Bullet::Bullet(const Position origin, const Position target)
    : position(origin), target(target) {
    const float dx = target.column - origin.column;
    const float dy = target.row - origin.row;
    direction.x = dx;
    direction.y = dy;
}

bool Bullet::move() {
    float distance = calculateDistance();
    if (distance > 0.1) {
        position.row += speed * direction.y;
        position.column += speed * direction.x;
        return false;
    }
    position = target;
    return true;
}

bool Bullet::checkCollision(const Position& other) const {
    return position.row == other.row && position.column == other.column;
}

Position Bullet::getPosition() const {
    return position;
}

float Bullet::calculateDistance() const {
    const float dx = target.column - position.column;
    const float dy = target.row - position.row;
    return std::sqrt(dx * dx + dy * dy);
}

// void Bullet::handleBounce() {
//
// }