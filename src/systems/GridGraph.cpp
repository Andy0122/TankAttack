#include "systems/GridGraph.h"
#include <cstdlib>
#include <ctime>
//#include <queue>
#include <random>
//#include <vector>
#include <algorithm>
//#include <unordered_set>
#include "data_structures/LinkedList.h"
#include "data_structures/DynamicArray.h"
#include "data_structures/Queue.h"
#include "data_structures/HashSet.h"

using namespace DATA_STRUCTURES;

/**
 * @brief Constructor de la clase Node.
 *
 * @param id Identificador único del nodo.
 * @param accessible Indica si el nodo es accesible (true por defecto).
 */
Node::Node(int id, bool accessible) : id(id), obstacle(accessible) {}

/**
 * @brief Constructor de la clase GridGraph.
 *
 * Inicializa la cuadrícula de nodos con un número predefinido de filas y columnas.
 * Los nodos son accesibles por defecto.
 */
GridGraph::GridGraph() {
    // Inicializar la cuadrícula de nodos con filas y columnas predefinidas
    int id = 0;
    grid.resize(rows);
    for (int row = 0; row < rows; ++row) {
        grid[row].resize(cols);
        for (int col = 0; col < cols; ++col) {
            grid[row][col] = Node(id++, true);  // Todos los nodos son accesibles por defecto
        }

    }

    // Inicializar la lista de adyacencia
    adjList.resize(rows * cols);
    std::cout << "adjList initialized with size: " << adjList.size() << std::endl;
    // Inicializar los nodos seguros
    int safeLeft[] = {75, 76, 100, 101, 102, 125, 126, 127, 150, 151, 152, 175, 176, 177, 200, 201, 202, 225, 226};
    int safeRight[] = {98, 99, 122, 123, 124, 147, 148, 149, 172, 173, 174, 197, 198, 199, 222, 223, 224, 248, 249};
    for (int i = 0; i < sizeof(safeLeft) / sizeof(int); ++i) {
        safeNodeIdsLeft.push_back(safeLeft[i]);
    }
    for (int i = 0; i < sizeof(safeRight) / sizeof(int); ++i) {
        safeNodeIdsRight.push_back(safeRight[i]);
    }
}

/**
 * @brief Metodo para obtener la lista de adyacencia del grafo.
 * @return Una referencia constante a la lista de adyacencia.
 */
const DynamicArray<LinkedList<int>>& GridGraph::getAdjList() const {
    return adjList;
}

/**
 * @brief Convierte las coordenadas 2D a un índice 1D en la cuadrícula.
 *
 * @param row Fila del nodo.
 * @param col Columna del nodo.
 * @return Índice 1D correspondiente al nodo.
 */
int GridGraph::toIndex(int row, int col) const {
    return row * cols + col;
}
/**
 * @brief Conecta los nodos accesibles en la cuadrícula.
 *
 * Conecta cada nodo con sus vecinos (arriba, abajo, izquierda, derecha) si son accesibles.
 */
void GridGraph::connectNodes() {
    std::cout << "adjList size before connecting nodes: " << adjList.size() << std::endl;
    if (!adjList.empty()) {
        // Acceder a adjList[currentIndex]
    } else {
        std::cerr << "adjList está vacía en connectNodes()" << std::endl;
    }
    // Asegurar que adjList tiene el tamaño correcto
    if (adjList.size() != rows * cols) {
        adjList.resize(rows * cols);
    }

    // Limpiar la lista de adyacencia antes de reconstruirla
    for (int i = 0; i < adjList.size(); ++i) {
        adjList[i].clear();
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Node& currentNode = grid[row][col];
            if (!currentNode.obstacle) {
                continue; // Si el nodo no es accesible, no se conecta
            }

            int currentIndex = toIndex(row, col);

            // Conectar al vecino de arriba
            if (row > 0 && grid[row - 1][col].obstacle) {
                int upIndex = toIndex(row - 1, col);
                adjList[currentIndex].push_back(upIndex);
            }

            // Conectar al vecino de abajo
            if (row < rows - 1 && grid[row + 1][col].obstacle) {
                int downIndex = toIndex(row + 1, col);
                adjList[currentIndex].push_back(downIndex);
            }

            // Conectar al vecino de la izquierda
            if (col > 0 && grid[row][col - 1].obstacle) {
                int leftIndex = toIndex(row, col - 1);
                adjList[currentIndex].push_back(leftIndex);
            }

            // Conectar al vecino de la derecha
            if (col < cols - 1 && grid[row][col + 1].obstacle) {
                int rightIndex = toIndex(row, col + 1);
                adjList[currentIndex].push_back(rightIndex);
            }
        }
    }
}

/**
 * @brief Modifica la accesibilidad de un nodo en la cuadrícula.
 *
 * @param row Fila del nodo.
 * @param col Columna del nodo.
 * @param accessible Valor booleano que indica si el nodo es accesible o no.
 */
void GridGraph::setNodeAccessibility(int row, int col, bool accessible) {
    grid[row][col].obstacle = accessible;
}

bool GridGraph::isObstacle(int row, int col) const {
    return !grid[row][col].obstacle;
}

bool GridGraph::isOccupied(int row, int col) const {
    return grid[row][col].occupied;
}

void GridGraph::placeTank(const int row, const int col) {
    grid[row][col].occupied = true;
}

void GridGraph::removeTank(int row, int col) {
    grid[row][col].occupied = false;
}

/**
 * @brief Imprime la representación del grafo, mostrando las conexiones entre nodos.
 */
void GridGraph::printGraph() const {
    for (int i = 0; i < adjList.size(); ++i) {
        std::cout << "Node " << i << " is connected to: ";
        const LinkedList<int>& neighbors = adjList[i];
        for (int j = 0; j < neighbors.size(); ++j) {
            int neighbor = neighbors[j];
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Imprime la accesibilidad de los nodos en la cuadrícula.
 *
 * Imprime un "1" si el nodo es accesible y "0" si no lo es.
 */
void GridGraph::printAccessibility() const {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            std::cout << (grid[row][col].obstacle ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Devuelve el número de filas de la cuadrícula.
 *
 * @return Número de filas.
 */
int GridGraph::getRows() const {
    return rows;
}

/**
 * @brief Devuelve el número de columnas de la cuadrícula.
 *
 * @return Número de columnas.
 */
int GridGraph::getCols() const {
    return cols;
}

/**
 * @brief Devuelve una referencia constante a un nodo específico en la cuadrícula.
 *
 * @param row Fila del nodo.
 * @param col Columna del nodo.
 * @return Referencia constante al nodo.
 */
const Node& GridGraph::getNode(int row, int col) const {
    return grid[row][col];
}

/**
 * @brief Verifica si un nodo es un nodo seguro.
 *
 * Un nodo seguro no puede ser inaccesible.
 *
 * @param nodeId ID del nodo a verificar.
 * @return true Si el nodo es seguro.
 * @return false Si el nodo no es seguro.
 */
bool GridGraph::isSafeNode(int nodeId) const{

    for (int i = 0; i < safeNodeIdsRight.size(); ++i) {
        int safeNode = safeNodeIdsRight[i];
        if (safeNode == nodeId) {
            return true;
        }
    }

    for (int i = 0; i < safeNodeIdsLeft.size(); ++i) {
        int safeNode = safeNodeIdsLeft[i];
        if (safeNode == nodeId) {
            return true;
        }
    }


    return false;
}

struct ObstacleShape {
    std::vector<std::pair<int, int>> cells; // Posiciones relativas de la forma
};

static const std::vector<ObstacleShape> predefinedShapes = {
    // Forma horizontal de longitud 3
    { { {0, 0}, {0, 1}, {0, 2} } },
    // Forma vertical de longitud 3
    { { {0, 0}, {1, 0}, {2, 0} } },
    // Forma en L
    { { {0, 0}, {1, 0}, {1, 1} } },
    // Cuadrado de 2x2
    { { {0, 0}, {0, 1}, {1, 0}, {1, 1} } },
    // Forma en T
    { { {0, -1}, {0, 0}, {0, 1}, {1, 0} } }
};


Position GridGraph::getRandomAccessiblePosition() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDis(0, rows - 1);
    std::uniform_int_distribution<> colDis(0, cols - 1);

    auto randomPosition = Position(rowDis(gen), colDis(gen));
    while (isObstacle(randomPosition.row, randomPosition.column)
        || isOccupied(randomPosition.row, randomPosition.column)) {
        randomPosition.row = rowDis(gen);
        randomPosition.column = colDis(gen);
    }

    return randomPosition;
}


void GridGraph::generateObstacles() {
    // Inicializar random seed
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> shapeDis(0, predefinedShapes.size() - 1);
    std::uniform_int_distribution<> rowDis(0, rows - 1);
    std::uniform_int_distribution<> colDis(0, cols - 1);

    // Número de obstáculos a generar
    int numObstacles = 10 + std::rand() % 10;

    int attempts = 0;
    int maxAttempts = 1000; // Para evitar bucles infinitos
    int placedObstacles = 0;

    while (placedObstacles < numObstacles && attempts < maxAttempts) {
        attempts++;

        // Seleccionar una forma aleatoria
        const ObstacleShape& shape = predefinedShapes[shapeDis(gen)];

        // Seleccionar una posición inicial aleatoria
        int baseRow = rowDis(gen);
        int baseCol = colDis(gen);

        // Verificar si la forma cabe en la posición y no interfiere con nodos seguros
        bool canPlace = true;
        std::vector<std::pair<int, int>> absolutePositions;

        for (const auto& cell : shape.cells) {
            int row = baseRow + cell.first;
            int col = baseCol + cell.second;

            // Verificar límites
            if (row < 0 || row >= rows || col < 0 || col >= cols) {
                canPlace = false;
                break;
            }

            int nodeId = toIndex(row, col);

            // Verificar si es un nodo seguro
            if (isSafeNode(nodeId)) {
                canPlace = false;
                break;
            }

            absolutePositions.emplace_back(row, col);
        }

        if (!canPlace) {
            continue;
        }

        // Colocar la forma en la cuadrícula
        for (const auto& pos : absolutePositions) {
            grid[pos.first][pos.second].obstacle = false; // Hacer inaccesible
        }

        placedObstacles++;
    }

    ensureObstaclesInLines();

    // Asegurar que existen grandes areas sin ningun obstaculo
    fillLargeOpenAreas();

    // Asegurar que existe al menos un obstáculo entre el extremo izquierdo y derecho
    ensureObstacleBetweenLeftAndRight();

    // Asegurar que los nodos seguros estén conectados entre sí
    ensureSafeNodesConnectivity();

    // Verificar que no se han creado áreas inaccesibles
    ensureNoIsolatedAreas();

    // Reconectar los nodos después de modificar la cuadrícula
    connectNodes();
}

/**
 * @brief Devuelve una referencia constante al nodo dado su ID.
 *
 * @param id Identificador único del nodo.
 * @return Referencia constante al nodo.
 */
const Node& GridGraph::getNodeById(int id) const {
    int row = id / cols;
    int col = id % cols;
    return grid[row][col];
}

/**
 * @brief Devuelve una referencia al nodo dado su ID.
 *
 * @param id Identificador único del nodo.
 * @return Referencia al nodo.
 */
Node& GridGraph::getNodeById(int id) {
    int row = id / cols;
    int col = id % cols;
    return grid[row][col];
}

void GridGraph::ensureSafeNodesConnectivity() {
    // Realizar BFS desde los nodos seguros de la izquierda
    Queue<int> q;
    HashSet<int> visited;

    // Agregar todos los nodos seguros de la izquierda al inicio del BFS
    for (int i = 0; i < safeNodeIdsLeft.size(); ++i) {
        int nodeId = safeNodeIdsLeft[i];
        q.push(nodeId);
        visited.insert(nodeId);
    }

    bool connected = false;

    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        // Si hemos llegado a un nodo seguro de la derecha, entonces están conectados
        for (int i = 0; i < safeNodeIdsRight.size(); ++i) {
            if (currentId == safeNodeIdsRight[i]) {
                connected = true;
                break;
            }
        }
        if (connected) {
            break;
        }

        int row = currentId / cols;
        int col = currentId % cols;

        // Vecinos (arriba, abajo, izquierda, derecha)
        LinkedList<int> neighbors;
        if (row > 0 && grid[row - 1][col].obstacle) neighbors.push_back(toIndex(row - 1, col));
        if (row < rows - 1 && grid[row + 1][col].obstacle) neighbors.push_back(toIndex(row + 1, col));
        if (col > 0 && grid[row][col - 1].obstacle) neighbors.push_back(toIndex(row, col - 1));
        if (col < cols - 1 && grid[row][col + 1].obstacle) neighbors.push_back(toIndex(row, col + 1));

        for (int i = 0; i < neighbors.size(); ++i) {
            int neighborId = neighbors[i];
            if (!visited.contains(neighborId)) {
                visited.insert(neighborId);
                q.push(neighborId);
            }
        }
    }

    if (!connected) {
        // Si no están conectados, necesitamos crear un camino
        // Tomamos un nodo seguro de la izquierda y uno de la derecha
        int leftNodeId = safeNodeIdsLeft[0];
        int rightNodeId = safeNodeIdsRight[0];

        int leftRow = leftNodeId / cols;
        int leftCol = leftNodeId % cols;
        int rightRow = rightNodeId / cols;
        int rightCol = rightNodeId % cols;

        // Crear un camino directo entre los nodos seguros
        int currentRow = leftRow;
        int currentCol = leftCol;

        while (currentRow != rightRow || currentCol != rightCol) {
            if (currentRow < rightRow) currentRow++;
            else if (currentRow > rightRow) currentRow--;
            if (currentCol < rightCol) currentCol++;
            else if (currentCol > rightCol) currentCol--;

            // Verificar que no sea un nodo seguro antes de modificar
            int nodeId = toIndex(currentRow, currentCol);
            if (!isSafeNode(nodeId)) {
                grid[currentRow][currentCol].obstacle = true; // Hacer accesible
            }
        }
    }
}


void GridGraph::ensureNoIsolatedAreas() {
    // Realizar BFS desde los nodos seguros para marcar nodos alcanzables
    Queue<int> q;
    HashSet<int> visited;

    // Agregar todos los nodos seguros al inicio del BFS
    for (int i = 0; i < safeNodeIdsLeft.size(); ++i) {
        int nodeId = safeNodeIdsLeft[i];
        q.push(nodeId);
        visited.insert(nodeId);
    }
    for (int i = 0; i < safeNodeIdsRight.size(); ++i) {
        int nodeId = safeNodeIdsRight[i];
        q.push(nodeId);
        visited.insert(nodeId);
    }

    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        int row = currentId / cols;
        int col = currentId % cols;

        // Vecinos (arriba, abajo, izquierda, derecha)
        LinkedList<int> neighbors;
        if (row > 0 && grid[row - 1][col].obstacle) neighbors.push_back(toIndex(row - 1, col));
        if (row < rows - 1 && grid[row + 1][col].obstacle) neighbors.push_back(toIndex(row + 1, col));
        if (col > 0 && grid[row][col - 1].obstacle) neighbors.push_back(toIndex(row, col - 1));
        if (col < cols - 1 && grid[row][col + 1].obstacle) neighbors.push_back(toIndex(row, col + 1));

        for (int i = 0; i < neighbors.size(); ++i) {
            int neighborId = neighbors[i];
            if (!visited.contains(neighborId)) {
                visited.insert(neighborId);
                q.push(neighborId);
            }
        }
    }

    // Convertir en obstáculos los nodos accesibles no visitados
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int nodeId = toIndex(row, col);
            if (grid[row][col].obstacle && !visited.contains(nodeId)) {
                grid[row][col].obstacle = false; // Hacer inaccesible
            }
        }
    }
}


void GridGraph::ensureObstacleBetweenLeftAndRight() {
    bool obstacleFound = false;

    for (int row = 0; row < rows; ++row) {
        for (int col = 1; col < cols - 1; ++col) {
            if (!grid[row][col].obstacle) {
                obstacleFound = true;
                break;
            }
        }
        if (obstacleFound) break;
    }

    if (!obstacleFound) {
        // Intentar colocar un obstáculo sin bloquear la conectividad
        int attempts = 0;
        const int maxAttempts = 1000;
        while (attempts < maxAttempts) {
            int randomRow = std::rand() % rows;
            int randomCol = 1 + std::rand() % (cols - 2); // Evitar las columnas de los extremos

            int nodeId = toIndex(randomRow, randomCol);
            if (!isSafeNode(nodeId)) {
                // Verificar si al colocar el obstáculo se mantiene la conectividad
                bool connectivityMaintained = true;

                // Temporariamente colocar el obstáculo
                grid[randomRow][randomCol].obstacle = false;
                connectNodes();

                // Verificar la conectividad
                if (!isConnected(safeNodeIdsLeft[0], safeNodeIdsRight[0])) {
                    connectivityMaintained = false;
                }

                // Restaurar el estado original si la conectividad se rompe
                if (!connectivityMaintained) {
                    grid[randomRow][randomCol].obstacle = true;
                } else {
                    // Obstáculo colocado sin romper la conectividad
                    break;
                }
            }
            attempts++;
        }
    }
}

bool GridGraph::isConnected(int startId, int goalId) {
    Queue<int> q;
    HashSet<int> visited;

    q.push(startId);
    visited.insert(startId);

    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        if (currentId == goalId) {
            return true;
        }

        const LinkedList<int>& neighbors = adjList[currentId];

        for (int i = 0; i < neighbors.size(); ++i) {
            int neighborId = neighbors[i];
            if (!visited.contains(neighborId)) {
                visited.insert(neighborId);
                q.push(neighborId);
            }
        }
    }

    return false;
}



void GridGraph::ensureObstaclesInLines() {
    // Asegurar que cada fila tenga al menos un obstáculo
    for (int row = 0; row < rows; ++row) {
        bool hasObstacle = false;
        for (int col = 0; col < cols; ++col) {
            if (!grid[row][col].obstacle) {
                hasObstacle = true;
                break;
            }
        }
        if (!hasObstacle) {
            // Agregar un obstáculo en una posición aleatoria de la fila, evitando zonas seguras
            int attempts = 0;
            const int maxAttempts = cols;
            while (attempts < maxAttempts) {
                int col = std::rand() % cols;
                int nodeId = toIndex(row, col);
                if (!isSafeNode(nodeId)) {
                    grid[row][col].obstacle = false;
                    break;
                }
                attempts++;
            }
        }
    }

    // Asegurar que cada columna tenga al menos un obstáculo
    for (int col = 0; col < cols; ++col) {
        bool hasObstacle = false;
        for (int row = 0; row < rows; ++row) {
            if (!grid[row][col].obstacle) {
                hasObstacle = true;
                break;
            }
        }
        if (!hasObstacle) {
            // Agregar un obstáculo en una posición aleatoria de la columna, evitando zonas seguras
            int attempts = 0;
            const int maxAttempts = rows;
            while (attempts < maxAttempts) {
                int row = std::rand() % rows;
                int nodeId = toIndex(row, col);
                if (!isSafeNode(nodeId)) {
                    grid[row][col].obstacle = false;
                    break;
                }
                attempts++;
            }
        }
    }
}

void GridGraph::fillLargeOpenAreas() {
    const int areaSize = 4; // Puedes ajustar este valor a 4 o 5 según tus necesidades

    for (int row = 0; row <= rows - areaSize; ++row) {
        for (int col = 0; col <= cols - areaSize; ++col) {
            bool isOpenArea = true;

            // Verificar si el área de areaSize x areaSize está libre de obstáculos
            for (int i = 0; i < areaSize; ++i) {
                for (int j = 0; j < areaSize; ++j) {
                    if (!grid[row + i][col + j].obstacle) {
                        isOpenArea = false;
                        break;
                    }
                }
                if (!isOpenArea) break;
            }

            if (isOpenArea) {
                // Colocar obstáculos en posiciones aleatorias dentro del área
                int obstaclesToPlace = areaSize; // Número de obstáculos a colocar
                int attempts = 0;
                const int maxAttempts = areaSize * areaSize * 2;
                while (obstaclesToPlace > 0 && attempts < maxAttempts) {
                    int i = std::rand() % areaSize;
                    int j = std::rand() % areaSize;
                    int nodeId = toIndex(row + i, col + j);
                    if (grid[row + i][col + j].obstacle && !isSafeNode(nodeId)) {
                        grid[row + i][col + j].obstacle = false;
                        obstaclesToPlace--;
                    }
                    attempts++;
                }
            }
        }
    }
}

bool GridGraph::isValid(const int row, const int column) {
    return row >= 0 && row < rows && column >= 0 && column < cols;
}
