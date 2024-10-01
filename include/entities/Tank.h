#ifndef TANK_H
#define TANK_H

#include "Position.h"

enum Color {
 Red, Yellow, Blue, Cian
};

/**
 * @class Tank
 * @brief Represents a tank with health, color, and position.
 * 
 * The Tank class contains attributes to define the tank's health, color, and position in a 2D space.
 * It also includes methods to modify and access these attributes.
 */
class Tank {
public:
   /**
    * @brief Constructor for the Tank Class
    *
    * Initialize an empty tank
    */
    Tank();

    /**
     * @brief Constructor for the Tank class.
     * 
     * Initializes a tank with the given health, color, and position.
     *
     * @param color Tank color.
     * @param position Initial tank position.
     */
    Tank(Color color, Position position);

    /**
     * @brief Gets the current health of the tank.
     * 
     * @return The health of the tank as an integer.
     */
    [[nodiscard]] int getHealth() const;

    /**
    * @brief Gets the current color of the tank.
    *
    * @return The color of the tank as a string.
    */
    [[nodiscard]] Color getColor() const;

    /**
     * @brief Gets the current row where the tank is placed.
     *
     * @return The row number.
     */
    [[nodiscard]] int getRow() const;

    /**
     * @brief Gets the current row where the tank is placed.
     *
     * @return The column number.
     */
    [[nodiscard]] int getColumn() const;

    /**
     * @brief Checks if the tank is selected.
     *
     * @return True if the tank is selected, false otherwise.
     */
    [[nodiscard]] bool isSelected() const;

    /**
     * @brief Sets the health of the tank.
     * 
     * @param newHealth New health value to set.
     */
    void setHealth(int newHealth);

    /**
    * @brief Sets the color of the tank.
    *
    * @param newColor New color value to set.
    */
    void setColor(Color newColor);

    /**
    * @brief Sets the position of the tank.
    *
    * @param newPosition New position where the tank will be placed.
    */
    void setPosition(Position newPosition);

    /**
     * @brief Sets the selected state of the tank.
     *
     * @param newState New state for selected.
     */
    void setSelected(bool newState);

private:
    int health{}; ///< The health of the tank (integer value).
    Color color; ///< The color of the tank (as a string).
    int row, column; ///< The cell position of the tank in the grid map.
    bool selected = false; ///< Indicates if the tank is selected.
};

#endif // TANK_H