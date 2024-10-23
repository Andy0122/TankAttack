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

private:
    Model* model; ///< Model of the game
    // View* view; ///< View of the game
};
