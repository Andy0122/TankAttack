#ifndef TANK_H
#define TANK_H

#include "Player.h"
#include "Position.h"


/**
 * @enum Color Represents the color of the tank.
 */
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
     * @param player Player that owns the tank.
     */
    Tank(Color color, Position position, Player* player);

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
    * @brief Gets the rotation angle of the tank.
    *
    * @return The rotation angle of the tank.
    */
    [[nodiscard]] double getRotationAngle() const;

    /**
    * @brief Gets the player that owns the tank.
    *
    * @return The player that owns the tank.
    */
    [[nodiscard]] Player* getPlayer() const;

    /**
    * @brief Sets the position of the tank.
    *
    * @param newPosition New position where the tank will be placed.
    */
    void setPosition(Position newPosition);

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
     * @brief Sets the selected state of the tank.
     *
     * @param newState New state for selected.
     */
    void setSelected(bool newState);

     /**
     * @brief Sets the rotation angle of the tank.
     *
     * @param angle The rotation angle to set.
     */
     void setRotationAngle(double angle);

    /**
     * @brief Applies damage to the tank.
     *
     * The damage applied depends on the tank's color.
     *
     * @param maxDamage True if the tank receives the maximum damage.
     */
    void applyDamage(bool maxDamage);

    /**
     * @brief Checks if the tank is destroyed.
     *
     * @return True if the tank is destroyed, false otherwise.
     */
    [[nodiscard]] bool isDestroyed() const;

    /**
     * @brief Sets the tank as destroyed.
     */
    void destroy();

private:
    int health; ///< The health of the tank (integer value).
    Color color; ///< The color of the tank (as a string).
    int row, column; ///< The cell position of the tank in the grid map.
    bool selected = false; ///< Indicates if the tank is selected.
    double rotationAngle = 0.0; ///< The rotation angle of the tank.
    Player* player; ///< The player that owns the tank.
    bool destroyed = false; ///< Indicates if the tank is destroyed.

    static constexpr int TANK_FULL_HEALTH = 100; ///< The full health of the tank.
};

#endif // TANK_H