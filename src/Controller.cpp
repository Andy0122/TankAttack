#include "Controller.h"

Controller::Controller(Model* model)
    : model(model) {}


GridGraph* Controller::getMap() const {
    return model->getMap();
}

Tank* Controller::getTanks() const {
    return model->getTanks();
}
