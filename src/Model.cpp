#include "Model.h"

Model::Model() {
    map = new GridGraph();
    map->generateObstacles();
    map->connectNodes();
    tank = new Tank(100, "yellow", 0, 5);
}

GridGraph* Model::getMap() const {
    return map;
}

Tank* Model::getTank() const {
    return tank;
}