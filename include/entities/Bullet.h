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

    void calculateDirection(Position origin, Position target);

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

    /**
     * @brief Gets the position of the bullet.
     *
     * @return The position of the bullet.
     */
    [[nodiscard]] Position getPosition() const;

    [[nodiscard]] Direction getDirection() const;

    void setDirection(Direction newDirection);

private:
    Position position; ///< The position of the bullet.
    Position target; ///< The position where the bullet is aimed.
    Direction direction{}; ///< The direction of the bullet.
    int distance;
    int speed = 1; ///< The speed of the bullet.
    float bounceAngle = 0.0; ///< The angle of the bounce.

    /**
     * @brief Calculates the distance between the bullet and the target.
     *
     * @return The distance between the bullet and the target.
     */
    [[nodiscard]] int calculateDistance() const;
};



#endif //BULLET_H
