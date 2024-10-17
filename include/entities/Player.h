#ifndef PLAYER_H
#define PLAYER_H

enum POWER_UP {
    DOUBLE_TURN, MOVEMENT_PRECISION, ATTACK_PRECISION, ATTACK_POWER, NONE
};

class Player {
public:
    explicit Player(int id);

    [[nodiscard]] int getId() const;

    [[nodiscard]] POWER_UP getPowerUp() const;

    void generatePowerUp();

private:
    int id;
    POWER_UP powerUp;
};



#endif //PLAYER_H
