#include "entities/Bullet.h"
#include <cmath>

using namespace DATA_STRUCTURES;

Bullet::Bullet(const Position origin, const Position target)
    : position(origin), target(target) {
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

Queue* Bullet::getPath() const {
    return path;
}

bool Bullet::reachedTarget() const {
    return path->empty();
}

void Bullet::setDirection(const Direction newDirection) {
    direction = newDirection;
}

void Bullet::setMaxDamage(const bool maxDamage) {
    this->maxDamage = maxDamage;
}

void Bullet::setPath(Queue& path) {
    this->path = &path;
}

void Bullet::setPath(Stack& path) {
    this->path = new Queue(); // Initialize the path queue
    while (!path.empty()) {
        this->path->push(path.top());
        path.pop();
    }
}

bool Bullet::move() {
    if (!path->empty()) {
        auto [row, column] = path->front();
        position.row = row;
        position.column = column;

        path->pop();

        return false;
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