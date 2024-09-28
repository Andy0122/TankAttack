#pragma once

#include "entities/Tank.h"
#include "systems/GridGraph.h"


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

    [[nodiscard]] Tank* getTank() const;

private:
    GridGraph* map; ///< Map of the game
    Tank* tank; ///< Temp tank
};
