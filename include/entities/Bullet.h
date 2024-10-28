#ifndef BULLET_H
#define BULLET_H

#include "Position.h"


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
     * @brief Sets if the bullet applies max damage.
     *
     * @param maxDamage True if the bullet applies max damage, false otherwise.
     */
    void setMaxDamage(bool maxDamage);

    void setRotationAngle(double angle);

    void setPosition(Position position);


private:
    Position position; ///< The position of the bullet.
    Position target; ///< The position where the bullet is aimed.
    double rotationAngle = 0.0; ///< The rotation angle of the bullet.
    bool maxDamage = false; ///< If the bullet applies max damage.
};



#endif //BULLET_H
