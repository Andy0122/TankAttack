#include "entities/Bullet.h"
#include <cmath>

Bullet::Bullet(const Position origin, const Position target)
    : position(origin), target(target) {
    calculateDirection(origin, target);
}

void Bullet::calculateDirection(const Position origin, const Position target) {
    int x;
    int y;

    if (target.column > origin.column) {
        x = 1;
    } else if (target.column < origin.column) {
        x = -1;
    } else {
        x = 0;
    }

    if (target.row > origin.row) {
        y = 1;
    } else if (target.row < origin.row) {
        y = -1;
    } else {
        y = 0;
    }

    direction = Direction(x, y);
}

bool Bullet::move() {
    if (const float distance = calculateDistance(); distance > 0.1) {
        if (position.row != target.row) {
            position.row += speed * direction.y;
        }
        if (position.column != target.column) {
            position.column += speed * direction.x;
        }

        return false;
    }

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