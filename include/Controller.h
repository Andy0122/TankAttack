#pragma once

#include "Model.h"
#include "systems/GridGraph.h"


/**
* @brief Controller class
*/
class Controller {
public:
    /**
    * @brief Controller Constructor
    *
    * @param model Model of the game
    */
    explicit Controller(Model* model);

    /**
    * @brief Gets the map of the game
    * @return GridGraph* Map of the game
    */
    [[nodiscard]] GridGraph* getMap() const;

    /**
    * @brief Gets the tanks of the game
    */
    [[nodiscard]] Tank* getTanks() const;

    [[nodiscard]] Tank* getTankOnPosition(Position position) const;

    [[nodiscard]] Tank* getSelectedTank() const;

    bool allTanksDestroyed(const Player* player) const;

    void handleSelectTank(Tank* tank) const;

    void handleMoveTank(const Tank* tank, Position position) const;

    void moveTank(Tank* tank, Position position) const;

    /**
    * @brief Gets the players of the game
    */
    [[nodiscard]] Player* getPlayers() const;

    /**
    * @brief Gets the current player
    */
    [[nodiscard]] Player* getCurrentPlayer() const;

    /**
    * @brief Gets the bullet of the game
    */
    [[nodiscard]] Bullet* getBullet() const;

    void handleFireBullet(Position src, Position dest) const;

    static void moveBullet(Bullet* bullet, Position position) ;

    [[nodiscard]] bool bulletHitTank() const;

    void handleBulletCollision() const;

    void handleTankDestruction(Tank* tank) const;

    void destroyBullet() const;

    void generatePowerUps() const;

    void handlePowerUpActivation() const;

    [[nodiscard]] int getRemainingTime() const;

    void decreaseTime() const;

    [[nodiscard]] Player* determineWinner() const;

    /**
    * @brief Gets the path of the tank
    */
    [[nodiscard]] DATA_STRUCTURES::Queue<Position>* getTankPath() const;

    [[nodiscard]] bool getGameOver() const;

    void setGameOver(bool gameOver) const;

private:
    Model* model; ///< Model of the game
};
