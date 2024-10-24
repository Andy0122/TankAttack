#include "entities/Bullet.h"

using namespace DATA_STRUCTURES;

Bullet::Bullet(const Position origin, const Position target)
    : position(origin), target(target) {}

Position Bullet::getPosition() const {
    return position;
}

bool Bullet::getMaxDamage() const {
    return maxDamage;
}

Queue<Position>* Bullet::getPath() const {
    return path;
}

bool Bullet::reachedTarget() const {
    return path->empty();
}

void Bullet::setMaxDamage(const bool maxDamage) {
    this->maxDamage = maxDamage;
}

void Bullet::setPath(Queue<Position>& path) {
    this->path = &path;
}

void Bullet::setPath(Stack<Position>& path) {
    this->path = new Queue<Position>(); // Initialize the path queue
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