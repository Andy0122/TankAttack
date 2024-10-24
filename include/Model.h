#pragma once

#include <entities/Bullet.h>

#include "data_structures/LinkedList.h"
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

    [[nodiscard]] Tank* getSelectedTank() const;

    /**
    * @brief Handles the selection of a tank
    *
    * @param tank Tank to select
    */
    void handleSelectTank(Tank* tank) const;

    /**
    * @brief Deselects all tanks
    */
    void deselectAllTanks() const;

    /**
    * @brief Handles the movement of a tank
    *
    * @param tank Tank to move
    * @param dest Destination of the tank
    */
    void handleMoveTank(const Tank* tank, Position dest);

    [[nodiscard]] DATA_STRUCTURES::Queue<Position>* calculatePath(Color color, POWER_UP powerUp, Position src, Position dest) const;

    static int calculateProbability(Color color, POWER_UP powerUp);

    /**
    * @brief Gets the players of the game
    */
    [[nodiscard]] Player* getPlayers() const;

    /**
    * @brief Gets the current player
    */
    [[nodiscard]] Player* getCurrentPlayer() const;

    /**
    * @brief Gets the current bullet
    */
    [[nodiscard]] Bullet* getBullet() const;

    /**
    * @brief Gets the path of the tank
    */
    [[nodiscard]] DATA_STRUCTURES::Queue<Position>* getTankPath() const;

    /**
    * @brief Sets the current player
    *
    * @param player Player to set as current
    */
    void setCurrentPlayer(Player* player);

    void moveTank(Tank* tank, Position position) const;

private:
    GridGraph* map = nullptr; ///< Map of the game
    Player* players = nullptr; ///< Array of players
    Tank* tanks = nullptr; ///< Array of tanks
    Player* currentPlayer = nullptr; ///< Current player
    Bullet* bullet = nullptr; ///< Current bullet on map

    DATA_STRUCTURES::Queue<Position>* tankPath = nullptr; ///< Path of the tank to move

    int actionsRemaining = 1;


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

    void decreaseActions();
};
