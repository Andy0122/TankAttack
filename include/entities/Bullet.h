#ifndef BULLET_H
#define BULLET_H

#include "Position.h"
#include "data_structures/Queue.h"
#include "data_structures/Stack.h"


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
     * @brief Gets if the bullet applies max damage.
     *
     * @return True if the bullet applies max damage, false otherwise.
     */
    [[nodiscard]] bool getMaxDamage() const;

    [[nodiscard]] double getRotationAngle() const;

    /**
     * @brief Gets the path the bullet has to follow.
     *
     * @return The path the bullet has to follow.
     */
    [[nodiscard]] DATA_STRUCTURES::Queue<Position>* getPath() const;

    /**
     * @brief Checks if the bullet has reached its target.
     *
     * @return True if the bullet has reached its target, false otherwise.
     */
    [[nodiscard]] bool reachedTarget() const;

    /**
     * @brief Sets if the bullet applies max damage.
     *
     * @param maxDamage True if the bullet applies max damage, false otherwise.
     */
    void setMaxDamage(bool maxDamage);

    void setRotationAngle(double angle);

    /**
     * @brief Sets the path the bullet has to follow.
     *
     * @param path The path the bullet has to follow.
     */
    void setPath(DATA_STRUCTURES::Queue<Position>& path);

    /**
     * @brief Sets the path the bullet has to follow.
     *
     * @param path The path the bullet has to follow.
     */
    void setPath(DATA_STRUCTURES::Stack<Position>& path);

    /**
    * @brief Moves the bullet.
    *
    * @return True if the bullet is still moving, false otherwise.
    */
    [[nodiscard]] bool move();

    void setPosition(Position position);

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
    double rotationAngle = 0.0; ///< The rotation angle of the bullet.
    DATA_STRUCTURES::Queue<Position>* path = nullptr; ///< The path the bullet has to follow.
    bool maxDamage = false; ///< If the bullet applies max damage.
};



#endif //BULLET_H
