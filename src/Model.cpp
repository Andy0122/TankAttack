#include "Model.h"

Model::Model() {
    map = new GridGraph();
    map->generateObstacles();
    map->connectNodes();

    tanks = new Tank[8];

    for (int i = 0; i < 8; i++) {
        const auto [row, col] = map->getRandomAccessiblePosition();
        // create a tank
        const auto tank = Tank(Cian, Position(row, col));
        // Place the tank in the map
        map->placeTank(row, col);
        tanks[i] = tank;
    }
}

GridGraph* Model::getMap() const {
    return map;
}

Tank* Model::getTanks() const {
    return tanks;
}