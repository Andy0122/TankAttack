#include "entities/Bullet.h"
#include <cmath>

Bullet::Bullet(const Position origin, const Position target, const bool maxDamage)
    : position(origin), target(target), maxDamage(maxDamage) {
    calculateDirection(origin, target);
    distance = calculateDistance();
}

Position Bullet::getPosition() const {
    return position;
}

Direction Bullet::getDirection() const {
    return direction;
}

int Bullet::getDistance() const {
    return distance;
}

bool Bullet::getMaxDamage() const {
    return maxDamage;
}

void Bullet::setDirection(const Direction newDirection) {
    direction = newDirection;
}

bool Bullet::move() {
    if (distance != 0) {
        if (position.row != target.row) {
            position.row += speed * direction.y;
        }
        if (position.column != target.column) {
            position.column += speed * direction.x;
        }

        distance -= 1;

        return distance == 0;
    }

    return true;
}

bool Bullet::bulletCollision(const Position& other) const {
    return position.row == other.row && position.column == other.column;
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

int Bullet::calculateDistance() const {
    const auto dx = static_cast<float>(target.column - position.column);
    const auto dy = static_cast<float>(target.row - position.row);
    return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}