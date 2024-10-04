#include "Model.h"

Model::Model() {
    map = new GridGraph();
    map->generateObstacles();
    map->connectNodes();
}

GridGraph* Model::getMap() const {
    return map;
}