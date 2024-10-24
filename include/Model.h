#pragma once

#include <entities/Bullet.h>
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

    [[nodiscard]] Tank* getTankOnPosition(Position position) const;

    bool allTanksDestroyed(const Player* player) const;

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

    [[nodiscard]] int calculateProbability(Color color, POWER_UP powerUp) const;

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

    void handleFireBullet(Position src, Position dest);

    static void moveBullet(Bullet* bullet, Position position);

    [[nodiscard]] bool bulletHitTank() const;

    void handleBulletCollision() const;

    static bool tankKilled(const Tank* tank);

    void handleTankDestruction(Tank* tank) const;

    void createBullet(Position src, Position dest, POWER_UP powerUp);

    void destroyBullet();

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

    void generatePowerUps() const;

    void handlePowerUpActivation();

    [[nodiscard]] int getRemainingTime() const;

    void decreaseTime();

    void endTurn();

    [[nodiscard]] Player* determineWinner() const;

    int getRemainingTanks(const Player* player) const;

    [[nodiscard]] bool getGameOver() const;

    void setGameOver(bool gameOver);

private:
    GridGraph* map = nullptr; ///< Map of the game
    Player* players = nullptr; ///< Array of players
    Tank* tanks = nullptr; ///< Array of tanks
    Player* currentPlayer = nullptr; ///< Current player
    Bullet* bullet = nullptr; ///< Current bullet on map

    DATA_STRUCTURES::Queue<Position>* tankPath = nullptr; ///< Path of the tank to move

    int actionsRemaining = 1; ///< Actions remaining for the current player

    int remainingTime = 300; ///< Remaining time of the game
    bool gameOver = false; ///< Indicates if the game is over


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
