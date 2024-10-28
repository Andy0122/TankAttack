#ifndef PLAYER_H
#define PLAYER_H

#include <string>


/**
 * Enum representing the power-ups that a player can have.
 */
enum POWER_UP {
    DOUBLE_TURN, MOVEMENT_PRECISION, ATTACK_PRECISION, ATTACK_POWER, NONE
};

/**
 * Class representing a player in the game.
 */
class Player {
public:
    /**
     * Constructor for the Player class.
     *
     * @param id The id of the player.
     */
    explicit Player(int id);

    /**
     * Getter for the id of the player.
     *
     * @return The id of the player.
     */
    [[nodiscard]] int getId() const;

    /**
     * Getter for the power-up of the player.
     *
     * @return The power-up of the player.
     */
    [[nodiscard]] POWER_UP getPowerUp() const;

    /**
     * Getter for the name of the power-up of the player.
     *
     * @return The name of the power-up of the player.
     */
    [[nodiscard]] std::string getPowerUpName() const;

    /**
     * Getter for the power-up active status of the player.
     *
     * @return The power-up active status of the player.
     */
    [[nodiscard]] bool getPowerUpActive() const;

    /**
     * Setter for the power-up active status of the player.
     *
     * @param active The power-up active status of the player.
     */
    void setPowerUpActive(bool active);

    /**
     * Erase the power-up of the player.
     */
    void erasePowerUp();

    /**
     * Generate a power-up for the player.
     */
    void generatePowerUp();

private:
    int id; ///< The id of the player.
    POWER_UP powerUp; ///< The power-up of the player.
    bool powerUpActive; ///< The power-up active status of the player.
};



#endif //PLAYER_H
