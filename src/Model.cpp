#include "Model.h"

Model::Model() {
    map = new GridGraph();
    map->generateObstacles();
    map->connectNodes();

    tanks = new Tank[8];
    Color color = Yellow;
    for (int i = 0; i < 8; i++) {
        const auto [row, col] = map->getRandomAccessiblePosition();
        if (i == 2) {
            color = Blue;
        } else if (i == 4) {
            color = Red;
        } else if (i == 6) {
            color = Cian;
        }
        // create a tank
        const auto tank = Tank(color, Position(row, col));
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