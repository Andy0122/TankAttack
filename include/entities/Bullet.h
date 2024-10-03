#ifndef BULLET_H
#define BULLET_H

#include "Position.h"

/**
 * @brief Represents the direction of the bullet.
 */
struct Direction {
    float x;
    float y;
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
    [[nodiscard]] bool checkCollision(const Position& other) const;

    /**
     * @brief Gets the position of the bullet.
     *
     * @return The position of the bullet.
     */
    [[nodiscard]] Position getPosition() const;

private:
    Position position; ///< The position of the bullet.
    Position target; ///< The position where the bullet is aimed.
    Direction direction{}; ///< The direction of the bullet.
    float speed = 1.0; ///< The speed of the bullet.
    float bounceAngle = 0.0; ///< The angle of the bounce.
    int damage{}; ///< The bullet damage.

    /**
     * @brief Calculates the distance between the bullet and the target.
     *
     * @return The distance between the bullet and the target.
     */
    [[nodiscard]] float calculateDistance() const;
};



#endif //BULLET_H
