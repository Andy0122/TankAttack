#pragma once

#include "entities/Player.h"
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

    /**
    * @brief Gets the tanks of the game
    */
    [[nodiscard]] Tank* getTanks() const;

    /**
    * @brief Gets the players of the game
    */
    [[nodiscard]] Player* getPlayers() const;

    /**
    * @brief Gets the current player
    */
    [[nodiscard]] Player* getCurrentPlayer() const;

    /**
    * @brief Sets the current player
    *
    * @param player Player to set as current
    */
    void setCurrentPlayer(Player* player);

private:
    GridGraph* map = nullptr; ///< Map of the game
    Player* players = nullptr; ///< Array of players
    Tank* tanks = nullptr; ///< Array of tanks
    Player* currentPlayer = nullptr; ///< Current player

    /**
    * @brief Creates the map of the game
    */
    void createMap();

    /**
    * @brief Creates the players of the game
    */
    void createPlayers();

    /**
    * @brief Creates the tanks of the game
    */
    void createTanks();

    /**
    * @brief Places the tanks on the map
    */
    void placeTanksOnMap() const;
};
