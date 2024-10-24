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

Tank* Controller::getTankOnPosition(Position position) const {
    auto [row, column] = position;
    Tank* tanks = getTanks();

    for (int i = 0; i < 8; i++) {
        if (Tank* tank = &tanks[i];
            !tank->isDestroyed() &&
            row == tank->getRow() && column == tank->getColumn()) {
                return tank;
            }
    }

    return nullptr;
}

Tank* Controller::getSelectedTank() const {
    return model->getSelectedTank();
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

Queue<Position>* Controller::getTankPath() const {
    return model->getTankPath();
}