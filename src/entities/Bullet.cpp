#include "entities/Bullet.h"
#include <cmath>

Bullet::Bullet(const Position origin, const Position target)
    : position(origin), target(target) {
    const auto dx = static_cast<float>(target.column - position.column);
    const auto dy = static_cast<float>(target.row - position.row);
    direction.x = dx;
    direction.y = dy;
}

bool Bullet::move() {
    if (const float distance = calculateDistance(); distance > 0.1) {
        position.row += static_cast<int>(speed * direction.y);
        position.column += static_cast<int>(speed * direction.x);
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
    const auto dx = static_cast<float>(target.column - position.column);
    const auto dy = static_cast<float>(target.row - position.row);
    return std::sqrt(dx * dx + dy * dy);
}