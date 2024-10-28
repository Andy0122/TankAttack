#include "entities/Bullet.h"


Bullet::Bullet(const Position origin, const Position target)
    : position(origin), target(target) {}

Position Bullet::getPosition() const {
    return position;
}

bool Bullet::getMaxDamage() const {
    return maxDamage;
}

double Bullet::getRotationAngle() const {
    return rotationAngle;
}

void Bullet::setMaxDamage(const bool maxDamage) {
    this->maxDamage = maxDamage;
}

void Bullet::setRotationAngle(const double angle) {
    this->rotationAngle = angle;
}

void Bullet::setPosition(const Position position) {
    this->position = position;
}