#include "Model.h"

Model::Model() {
    map = new GridGraph();
    map->generateObstacles();
    map->connectNodes();

    // Definir los tanques y sus posiciones
    tanks = new Tank[8];

    // Lista de pares (ID, Color)
    std::vector<std::pair<int, Color>> tankPositions = {
        {100, Red},
        {126, Red},
        {176, Blue},
        {200, Blue},
        {124, Yellow},
        {148, Yellow},
        {198, Cian},
        {224, Cian}
    };

    for (int i = 0; i < tankPositions.size(); ++i) {
        int id = tankPositions[i].first;
        Color color = tankPositions[i].second;

        // Convertir ID a coordenadas (row, col)
        int row = id / map->getCols();
        int col = id % map->getCols();

        // Verificar si la posición es accesible y no está ocupada
        if (map->isObstacle(row, col) || map->isOccupied(row, col)) {
            std::cerr << "La posición (" << row << ", " << col << ") no es válida para colocar el tanque." << std::endl;
            continue;
        }

        // Crear el tanque
        Tank tank(color, Position(row, col));

        // Colocar el tanque en el mapa
        map->placeTank(row, col);

        // Almacenar el tanque en el arreglo
        tanks[i] = tank;
    }
}

GridGraph* Model::getMap() const {
    return map;
}

Tank* Model::getTanks() const {
    return tanks;
}