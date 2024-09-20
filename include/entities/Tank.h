#ifndef TANK_H
#define TANK_H

#include <string>
#include <iostream>

/**
 * @class Tank
 * @brief Represents a tank with health, color, and position.
 * 
 * The Tank class contains attributes to define the tank's health, color, and position in a 2D space.
 * It also includes methods to modify and access these attributes.
 */
class Tank {
private:
    int health;          ///< The health of the tank (integer value).
    std::string color;   ///< The color of the tank (as a string).
    float position[2];   ///< The position of the tank in 2D space (array of two floats: x, y).

public:
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
    Tank(int h, std::string c, float x, float y);

    /**
     * @brief Gets the current health of the tank.
     * 
     * @return The health of the tank as an integer.
     */
    int getHealth() const;

    /**
     * @brief Sets the health of the tank.
     * 
     * @param h New health value to set.
     */
    void setHealth(int h);

    /**
     * @brief Gets the current color of the tank.
     * 
     * @return The color of the tank as a string.
     */
    std::string getColor() const;

    /**
     * @brief Sets the color of the tank.
     * 
     * @param c New color value to set.
     */
    void setColor(std::string c);

    /**
     * @brief Gets the current position of the tank.
     * 
     * Displays the current x and y coordinates of the tank in the console.
     */
    void getPosition() const;

    /**
     * @brief Sets the position of the tank.
     * 
     * @param x New x coordinate.
     * @param y New y coordinate.
     */
    void setPosition(float x, float y);

    /**
     * @brief Displays the tank's information.
     * 
     * Prints the current health, color, and position of the tank to the console.
     */
    void displayInfo() const;
};

#endif // TANK_H