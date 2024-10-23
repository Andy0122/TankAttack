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

    /**
    * @brief Gets the players of the game
    */
    [[nodiscard]] Player* getPlayers() const;

    /**
    * @brief Gets the current player
    */
    [[nodiscard]] Player* getCurrentPlayer() const;

private:
    Model* model; ///< Model of the game
};
