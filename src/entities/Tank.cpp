#include "../../include/entities/Tank.h"


Tank::Tank(int h, std::string c, int row, int column) : health(h), color(c), row(row), column(column) {}


int Tank::getHealth() const {
    return health;
}


void Tank::setHealth(int h) {
    health = h;
}


std::string Tank::getColor() const {
    return color;
}


void Tank::setColor(std::string c) {
    color = c;
}


void Tank::getPosition() const {
    std::cout << "Cell Position: " << row * 21 + column << "\n";
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

void Tank::setPosition(int newRow, int newColumn) {
    row = newRow;
    column = newColumn;
}

void Tank::setSelected(const bool s) {
    selected = s;
}

void Tank::displayInfo() const {
    std::cout << "Tank Info: \n";
    std::cout << "Health: " << health << "\n";
    std::cout << "Color: " << color << "\n";
    std::cout << "Cell Position: " << row * 21 + column << "\n";
}

