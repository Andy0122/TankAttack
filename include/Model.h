#ifndef MODEL_H
#define MODEL_H

#include "systems/GridGraph.h"

class Model {
public:
    Model();
    [[nodiscard]] GridGraph* getMap() const;

private:
    GridGraph* map;
};

#endif //MODEL_H
