#ifndef PLAYER_H
#define PLAYER_H

#include "Tank.h"

enum POWER_UP {
    DOUBLE_TURN, MOVEMENT_PRECISION, ATTACK_PRECISION, ATTACK_POWER, NONE
};

class Player {
public:
    Player();

    void setTanks(Tank* tanks);

    [[nodiscard]] Tank* getTanks() const;

    [[nodiscard]] POWER_UP getPowerUp() const;

    void generatePowerUp();

private:
    Tank* tanks;
    POWER_UP powerUp;
};



#endif //PLAYER_H
