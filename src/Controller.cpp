#include "Controller.h"

Controller::Controller(Model* model)
    : model(model) {}


GridGraph* Controller::getMap() const {
    return model->getMap();
}

Tank* Controller::getTanks() const {
    return model->getTanks();
}

Player* Controller::getPlayers() const {
    return model->getPlayers();
}

Player* Controller::getCurrentPlayer() const {
    return model->getCurrentPlayer();
}