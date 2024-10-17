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

private:
    GridGraph* map; ///< Map of the game
    Player* players; ///< Array of players
    Tank* tanks; ///< Array of tanks
};
