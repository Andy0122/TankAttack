#pragma once

#include "entities/Player.h"
#include "entities/Tank.h"
#include "systems/GridGraph.h"

struct TankData {
    int id;
    Color color;
    int player;
};

/**
 * @brief Model class
 */
class Model {
public:
    /**
    * @brief Model Constructor
    */
    Model();

    /**
    *  @brief Gets the map of the game
    *  @returns GridGraph* Map of the game
    */
    [[nodiscard]] GridGraph* getMap() const;

    /**
    * @brief Gets the tanks of the game
    */
    [[nodiscard]] Tank* getTanks() const;

    [[nodiscard]] Player* getPlayers() const;

private:
    GridGraph* map = nullptr; ///< Map of the game
    Player* players = nullptr; ///< Array of players
    Tank* tanks = nullptr; ///< Array of tanks

    void createMap();

    void createPlayers();

    void createTanks();

    void placeTanksOnMap() const;
};
