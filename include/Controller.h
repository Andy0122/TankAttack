#pragma once

#include "View.h"
#include "Model.h"
#include "systems/GridGraph.h"


class Controller {
public:
    Controller(View* view, Model* model);
    [[nodiscard]] GridGraph* getMap() const;
    void updateGameState() const;

private:
    Model* model;
    View* view;
};
