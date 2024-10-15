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

        int playerNumber;
        if (color == Red || color == Blue) {
            playerNumber = 0;
        } else if (color == Yellow || color == Cian) {
            playerNumber = 1;
        }

        // Crear e inicializar el objeto tank con el constructor adecuado
        Tank tank(color, Position(row, col), playerNumber);


        // Colocar el tanque en el mapa
        map->placeTank(row, col);

        // Almacenar el tanque en el arreglo
        tanks[i] = tank;
    }

    for (int i = 0; i < 8; ++i) {
        if (tanks[i].getColor() == Yellow || tanks[i].getColor() == Cian) {
            tanks[i].setRotationAngle(180.0); // Girar 180 grados
        } else {
            tanks[i].setRotationAngle(0.0); // Sin rotación
        }
    }

}

GridGraph* Model::getMap() const {
    return map;
}

Tank* Model::getTanks() const {
    return tanks;
}