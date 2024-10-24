#include "Controller.h"

using namespace DATA_STRUCTURES;

Controller::Controller(Model* model)
    : model(model) {}


GridGraph* Controller::getMap() const {
    return model->getMap();
}

Tank* Controller::getTanks() const {
    return model->getTanks();
}

Tank* Controller::getTankOnPosition(const Position position) const {
    return model->getTankOnPosition(position);
}

Tank* Controller::getSelectedTank() const {
    return model->getSelectedTank();
}

bool Controller::allTanksDestroyed(const Player* player) const {
    return model->allTanksDestroyed(player);
}

void Controller::handleSelectTank(Tank* tank) const {
    model->handleSelectTank(tank);
}

void Controller::handleMoveTank(const Tank* tank, const Position position) const {
    model->handleMoveTank(tank, position);
}

void Controller::moveTank(Tank* tank, const Position position) const {
    model->moveTank(tank, position);
}

Player* Controller::getPlayers() const {
    return model->getPlayers();
}

Player* Controller::getCurrentPlayer() const {
    return model->getCurrentPlayer();
}

Bullet *Controller::getBullet() const {
    return model->getBullet();
}

void Controller::handleFireBullet(const Position src, const Position dest) const {
    model->handleFireBullet(src, dest);
}

void Controller::moveBullet(Bullet* bullet, const Position position) {
    Model::moveBullet(bullet, position);
}

bool Controller::bulletHitTank() const {
    return model->bulletHitTank();
}

void Controller::handleBulletCollision() const {
    model->handleBulletCollision();
}

void Controller::handleTankDestruction(Tank* tank) const {
    model->handleTankDestruction(tank);
}

void Controller::destroyBullet() const {
    model->destroyBullet();
}

void Controller::generatePowerUps() const {
    model->generatePowerUps();
}


void Controller::handlePowerUpActivation() const {
    model->handlePowerUpActivation();
}


Queue<Position>* Controller::getTankPath() const {
    return model->getTankPath();
}

bool Controller::getGameOver() const {
    return model->getGameOver();
}

void Controller::setGameOver(const bool gameOver) const {
    model->setGameOver(gameOver);
}

int Controller::getRemainingTime() const {
    return model->getRemainingTime();
}

void Controller::decreaseTime() const {
    model->decreaseTime();
}

Player* Controller::determineWinner() const {
    return model->determineWinner();
}