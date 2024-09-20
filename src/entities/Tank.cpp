#include "../../include/entities/Tank.h"

/**
 * @brief Constructor for the Tank class.
 *
 * Initializes a tank with the given health, color, and position.
 *
 * @param h Initial health of the tank.
 * @param c Color of the tank.
 * @param x X coordinate of the tank's initial position.
 * @param y Y coordinate of the tank's initial position.
 */
Tank::Tank(int h, std::string c, float x, float y) {
    health = h;
    color = c;
    position[0] = x;
    position[1] = y;
}

/**
 * @brief Gets the current health of the tank.
 *
 * @return The health of the tank as an integer.
 */
int Tank::getHealth() const {
    return health;
}

/**
 * @brief Sets the health of the tank.
 *
 * @param h New health value to set.
 */
void Tank::setHealth(int h) {
    health = h;
}

/**
 * @brief Gets the current color of the tank.
 *
 * @return The color of the tank as a string.
 */
std::string Tank::getColor() const {
    return color;
}

/**
 * @brief Sets the color of the tank.
 *
 * @param c New color value to set.
 */
void Tank::setColor(std::string c) {
    color = c;
}

/**
 * @brief Gets the current position of the tank.
 *
 * Displays the current x and y coordinates of the tank in the console.
 */
void Tank::getPosition() const {
    std::cout << "Position: (" << position[0] << ", " << position[1] << ")\n";
}

/**
 * @brief Sets the position of the tank.
 *
 * @param x New x coordinate.
 * @param y New y coordinate.
 */
void Tank::setPosition(float x, float y) {
    position[0] = x;
    position[1] = y;
}

/**
 * @brief Displays the tank's information.
 *
 * Prints the current health, color, and position of the tank to the console.
 */
void Tank::displayInfo() const {
    std::cout << "Tank Info: \n";
    std::cout << "Health: " << health << "\n";
    std::cout << "Color: " << color << "\n";
    std::cout << "Position: (" << position[0] << ", " << position[1] << ")\n";
}

