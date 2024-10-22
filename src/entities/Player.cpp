#include "entities/Player.h"
#include <random>

Player::Player(const int id) : id(id), powerUp(ATTACK_PRECISION) {}

int Player::getId() const {
    return id;
}

POWER_UP Player::getPowerUp() const {
    return powerUp;
}

std::string Player::getPowerUpName() const {
    switch (powerUp) {
        case DOUBLE_TURN:
            return "Double Turn";
        case MOVEMENT_PRECISION:
            return "Movement Precision";
        case ATTACK_PRECISION:
            return "Attack Precision";
        case ATTACK_POWER:
            return "Attack Power";
        case NONE:
            return "None";
    }
    return "None";
}

void Player::generatePowerUp() {
    if (powerUp != NONE) {
        return;
    }

    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(0, 4);
    //
    // powerUp = static_cast<POWER_UP>(dis(gen));
    powerUp = ATTACK_PRECISION; // Temporal
}
