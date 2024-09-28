#pragma once

#include "View.h"
#include "Model.h"
#include "systems/GridGraph.h"


/**
* @brief Controller class
*/
class Controller {
public:
    /**
    * @brief Controller Constructor
    * @param view View of the game
    * @param model Model of the game
    */
    Controller(View* view, Model* model);

    /**
    * @brief Gets the map of the game
    * @return GridGraph* Map of the game
    */
    [[nodiscard]] GridGraph* getMap() const;

    /**
    * @brief Updates the game state
    */
    void updateGameState() const;

private:
    Model* model; ///< Model of the game
    View* view; ///< View of the game
};
