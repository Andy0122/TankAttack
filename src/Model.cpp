#include "Model.h"

Model::Model() {
    // Create the map
    createMap();

    // Create players
    createPlayers();

    // Set tanks positions
    createTanks();

    // Place Tanks on the map
    placeTanksOnMap();
}

GridGraph* Model::getMap() const {
    return map;
}

Tank* Model::getTanks() const {
    return tanks;
}

Player* Model::getPlayers() const {
    return players;
}

void Model::createMap() {
    map = new GridGraph();
    map->generateObstacles();
    map->connectNodes();
}

void Model::createPlayers() {
    players = new Player[2] {
        Player(0),
        Player(1)
    };
}

void Model::createTanks() {
    tanks = new Tank[8] {
        Tank(Red, Position(100 / map->getCols(), 100 % map->getCols()), &players[0]),
        Tank(Red, Position(126 / map->getCols(), 126 % map->getCols()), &players[0]),
        Tank(Blue, Position(176 / map->getCols(), 176 % map->getCols()), &players[0]),
        Tank(Blue, Position(200 / map->getCols(), 200 % map->getCols()), &players[0]),
        Tank(Yellow, Position(124 / map->getCols(), 124 % map->getCols()), &players[1]),
        Tank(Yellow, Position(148 / map->getCols(), 148 % map->getCols()), &players[1]),
        Tank(Cian, Position(198 / map->getCols(), 198 % map->getCols()), &players[1]),
        Tank(Cian, Position(224 / map->getCols(), 224 % map->getCols()), &players[1])
    };

    for (int i = 0; i < 8; ++i) {
        if (tanks[i].getColor() == Red || tanks[i].getColor() == Blue) {
            tanks[i].setRotationAngle(0.0);
        } else {
            tanks[i].setRotationAngle(180.0);
        }
    }
}

void Model::placeTanksOnMap() const {
    for (int i = 0; i < 8; ++i) {
        map->placeTank(tanks[i].getRow(), tanks[i].getColumn());
    }
}

