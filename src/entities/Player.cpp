#include "entities/Player.h"
#include <random>

Player::Player() : tanks(nullptr), powerUp(NONE) {}

void Player::setTanks(Tank* tanks) {
    this->tanks = tanks;
}

Tank* Player::getTanks() const {
    return tanks;
}

POWER_UP Player::getPowerUp() const {
    return powerUp;
}

void Player::generatePowerUp() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    powerUp = static_cast<POWER_UP>(dis(gen));
}