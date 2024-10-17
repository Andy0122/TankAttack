#include "entities/Tank.h"


Tank::Tank() : health(TANK_FULL_HEALTH), color(Color()), row(0), column(0), player(nullptr) {}

Tank::Tank(const Color color, const Position position, Player* player)
    : health(TANK_FULL_HEALTH), color(color), row(position.row), column(position.column), player(player) {}

int Tank::getHealth() const {
    return health;
}

Color Tank::getColor() const {
    return color;
}

int Tank::getRow() const {
    return row;
}

int Tank::getColumn() const {
    return column;
}

bool Tank::isSelected() const {
    return selected;
}

void Tank::setHealth(const int newHealth) {
    health = newHealth;
}

void Tank::setColor(const Color newColor) {
    color = newColor;
}

void Tank::setPosition(const Position newPosition) {
    row = newPosition.row;
    column = newPosition.column;
}

void Tank::setSelected(const bool newState) {
    selected = newState;
}

void Tank::applyDamage() {
    if (color == Cian || color == Blue) {
        health -= static_cast<int>(TANK_FULL_HEALTH * 0.25);
    } else {
        health -= static_cast<int>(TANK_FULL_HEALTH * 0.5);
    }
}

bool Tank::isDestroyed() const {
    return destroyed;
}

void Tank::destroy() {
    destroyed = true;
}
