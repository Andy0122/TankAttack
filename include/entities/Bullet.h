#ifndef BULLET_H
#define BULLET_H

#include "Position.h"

/**
 * @brief Represents a unitary vector for the direction of the bullet.
 * X and Y values are between -1 and 1.
 */
struct Direction {
    int x; ///< X value.
    int y; ///< Y value.
};

/**
 * @brief Represents a bullet in the game.
 */
class Bullet {
public:
    /**
     * @brief Creates a new bullet.
     *
     * @param origin The position where the bullet is created.
     * @param target The position where the bullet is aimed.
     */
    Bullet(Position origin, Position target);

    /**
     * @brief Destroys the bullet.
     */
    ~Bullet() = default;

    /**
     * @brief Gets the position of the bullet.
     *
     * @return The position of the bullet.
     */
    [[nodiscard]] Position getPosition() const;

    /**
     * @brief Gets the direction of the bullet.
     *
     * @return The direction of the bullet.
     */
    [[nodiscard]] Direction getDirection() const;

    /**
     * @brief Gets the distance the bullet has to travel.
     *
     * @return The distance the bullet has to travel.
     */
    [[nodiscard]] int getDistance() const;

    /**
     * @brief Sets the direction of the bullet.
     *
     * @param newDirection The new direction of the bullet.
     */
    void setDirection(Direction newDirection);

    /**
    * @brief Moves the bullet.
    *
    * @return True if the bullet is still moving, false otherwise.
    */
    [[nodiscard]] bool move();

    /**
     * @brief Checks if the bullet collides with another position.
     *
     * @param other The position to check collision with.
     * @return true if the bullet collides with the other position, false otherwise.
     */
    [[nodiscard]] bool bulletCollision(const Position& other) const;


private:
    Position position; ///< The position of the bullet.
    Position target; ///< The position where the bullet is aimed.
    Direction direction{}; ///< The direction of the bullet.
    int distance; ///< The distance the bullet has to travel.
    int speed = 1; ///< The speed of the bullet.
    float bounceAngle = 0.0; ///< The angle of the bounce.

    /**
     * @brief Calculates the direction of the bullet.
     *
     * @param origin The position where the bullet is created.
     * @param target The position where the bullet is aimed.
     */
    void calculateDirection(Position origin, Position target);

    /**
     * @brief Calculates the distance between the bullet and the target.
     *
     * @return The distance between the bullet and the target.
     */
    [[nodiscard]] int calculateDistance() const;
};



#endif //BULLET_H
