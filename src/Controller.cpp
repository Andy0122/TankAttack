#include "Controller.h"

Controller::Controller(View* view, Model* model)
    : model(model), view(view) {
    view->setGridMap(model->getMap());
    view->setTanks(model->getTanks());
    view->update();
}

GridGraph* Controller::getMap() const {
    return model->getMap();
}

void Controller::updateGameState() const {
    view->update();
}