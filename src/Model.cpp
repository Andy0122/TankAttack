#include "Model.h"

#include <random>
#include <View.h>
#include <systems/Pathfinder.h>

using namespace DATA_STRUCTURES;

Model::Model() {
    // Create the map
    createMap();

    // Create players
    createPlayers();
    setCurrentPlayer(&players[0]);

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

Tank* Model::getSelectedTank() const {
    for (int i = 0; i < 8; i++) {
        if (tanks[i].isSelected()) {
            return &tanks[i];
        }
    }

    return nullptr;
}

void Model::handleSelectTank(Tank* tank) const {
    const int playerId = tank->getPlayer()->getId();

    if (const int currentPlayerId = currentPlayer->getId();
        playerId != currentPlayerId || tank->isDestroyed()) {
        return;
    }

    deselectAllTanks();
    tank->setSelected(true);
}

void Model::deselectAllTanks() const {
    for (int i = 0; i < 8; i++) {
        tanks[i].setSelected(false);
    }
}

void Model::handleMoveTank(const Tank* tank, const Position dest) {
    if (tank->isDestroyed()) {
        return;
    }

    Player* tankPlayer = tank->getPlayer();
    const POWER_UP playerPowerUp = tankPlayer->getPowerUp();

    // Calculate path
    const auto src = Position{tank->getRow(), tank->getColumn()};
    tankPath = calculatePath(tank->getColor(), playerPowerUp, src, dest);

    // Erase power up
    if (tankPlayer->getPowerUpActive() && playerPowerUp == MOVEMENT_PRECISION) {
        tankPlayer->erasePowerUp();
    }

    // Decrement actions remaining
    decreaseActions();
}

Queue<Position>* Model::calculatePath(const Color color, const POWER_UP powerUp, Position src, Position dest) const {
    Pathfinder pathfinder(*getMap());

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10);
    const int randomNumber = dist(gen);

    const int probability = calculateProbability(color, powerUp);

    if (color == Red || color == Yellow) {
        if (randomNumber <= probability) {
            return pathfinder.bfs(src, dest);
        }
    } else {
        if (randomNumber <= probability) {
            // return pathfinder.dijkstra();
            return pathfinder.bfs(src, dest);
        }
    }
    // return pathfinder.randomMovement();
    return pathfinder.bfs(src, dest);

}

int Model::calculateProbability(const Color color, const POWER_UP powerUp) {
    if (powerUp == MOVEMENT_PRECISION) {
        return 9;
    }

    if (color == Red || color == Yellow) {
        return 5;
    }
    return 8;
}



Player* Model::getPlayers() const {
    return players;
}

Player* Model::getCurrentPlayer() const {
    return currentPlayer;
}

Bullet *Model::getBullet() const {
    return bullet;
}

Queue<Position>* Model::getTankPath() const {
    return tankPath;
}

void Model::setCurrentPlayer(Player* player) {
    currentPlayer = player;
}

void Model::moveTank(Tank* tank, const Position position) const {
    if (tank->isSelected()) {
        if (!map->isObstacle(position.row, position.column)
            && !map->isOccupied(position.row, position.column)) {
            map->removeTank(tank->getRow(), tank->getColumn());
            map->placeTank(position.row, position.column);

            tank->setPosition(position);
        }
    }
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

void Model::decreaseActions() {
    actionsRemaining--;

    if (actionsRemaining <= 0) {
        // end turn
    }
}


