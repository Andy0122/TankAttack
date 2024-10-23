#ifndef PLAYER_H
#define PLAYER_H

#include <string>

enum POWER_UP {
    DOUBLE_TURN, MOVEMENT_PRECISION, ATTACK_PRECISION, ATTACK_POWER, NONE
};

class Player {
public:
    explicit Player(int id);

    [[nodiscard]] int getId() const;

    [[nodiscard]] POWER_UP getPowerUp() const;

    [[nodiscard]] std::string getPowerUpName() const;

    [[nodiscard]] bool getPowerUpActive() const;

    void setPowerUpActive(bool active);

    void erasePowerUp();

    void generatePowerUp();

private:
    int id;
    POWER_UP powerUp;
    bool powerUpActive;
};



#endif //PLAYER_H
