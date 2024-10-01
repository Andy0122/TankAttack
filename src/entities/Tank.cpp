#include "entities/Tank.h"


Tank::Tank() : color(Color()), row(0), column(0) {}

Tank::Tank(const Color color, const Position position)
    : color(color), row(position.row), column(position.column) {}

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

